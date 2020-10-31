// written by Asuna Amawaka for FlareOn7 10_-_break

#define _GNU_SOURCE
#define __USE_GNU

#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/stat.h>

typedef int (*real_memcmp_t)(const void *, const void *, size_t);
typedef int (*real_strcmp_t)(const char*, const char*);
typedef int (*real_strncmp_t)(const char *, const char *, size_t);
typedef void *(*real_pivot_root_t)(const char *, const char *);
typedef int (*real_mlockall_t)(int);
typedef int (*real_uname_t)(struct utsname *);
typedef int (*real_chmod_t)(const char *, mode_t);
typedef void *(*real_truncate_t)(const char *, off_t);
typedef void (*real_memcpy_t)( void *, const void *, size_t);


void inspect_memory(int addr, int size) {
	printf("inside {0x%lx}: ", addr);

	unsigned char *p = (unsigned char *)addr;
	size_t i;
	for (i=0; i < size; ++i){
	    if (p[i]>=0x20 && p[i]<0x7F)
			printf("%c",p[i]);
	    else
	        printf("%02x", p[i]);
	}
	printf("\n");
}

int real_memcmp(const void * ptr1, const void * ptr2, size_t num) {
	return ((real_memcmp_t)dlsym(RTLD_NEXT, "memcmp"))(ptr1,ptr2,num);
}

int memcmp(const void * ptr1, const void * ptr2, size_t num) {
	int memcmp_ret = real_memcmp(ptr1, ptr2, num);
	printf("memcmp (%s, %s) = %d\n", (const char *)ptr1, (const char *)ptr2, memcmp_ret);
	return memcmp_ret;
}

int real_strcmp (const char* str1, const char* str2) {
	((real_strcmp_t)dlsym(RTLD_NEXT, "strcmp"))(str1, str2);
}

int strcmp (const char* str1, const char* str2) {
	int strcmp_ret = real_strcmp(str1,str2);
	printf("strcmp(%s, %s) = %d\n", str1, str2, strcmp_ret);
	return strcmp_ret;
}

int real_strncmp ( const char * str1, const char * str2, size_t num ) {
	((real_strncmp_t)dlsym(RTLD_NEXT, "strncmp"))(str1, str2, num);
}

int strncmp ( const char * str1, const char * str2, size_t num ) {
	int strncmp_ret = real_strncmp(str1, str2, num);
	printf("strncmp %d bytes (%s, %s) = %d\n", num, str1, str2,strncmp_ret);
	return strncmp_ret;

}

int real_truncate(const char *path, off_t length) {
	((real_truncate_t)dlsym(RTLD_NEXT, "truncate"))(path, length);

}

int truncate(const char *path, off_t length) {
	int truncate_ret = real_truncate(path, length);
	printf("truncate(%lx,%lx) = %d\n", path, length, truncate_ret);
	inspect_memory(0x804c648, 0x20);
	return truncate_ret;
}


void * real_memcpy ( void * destination, const void * source, size_t num ) {
	((real_memcpy_t)dlsym(RTLD_NEXT, "memcpy"))(destination, source, num);
}

void * memcpy ( void * destination, const void * source, size_t num ) {
	printf("memcpy %d bytes from SRC 0x%lx to DEST 0x%lx\n",num, source, destination);
	inspect_memory(destination, num);
	inspect_memory(source, num);

	if (destination == 0x804c640) {
		real_memcpy(destination, 0x81A5100, num+8);
	}
	else {
		real_memcpy(destination, source, num);
	}

	printf("after copy:\n");
	inspect_memory(destination, num);
	printf("----------------------\n");
}

int proot_count = 1;
int chmodloop_counter = 1;
int unameloop_counter = 1;
int block_counter = 0;

int real_pivot_root(const char *new_root, const char *put_old) {
	return ((real_pivot_root_t)dlsym(RTLD_NEXT, "pivot_root"))(new_root,put_old);
}

int pivot_root(const char *new_root, const char *put_old) {		
	int proot_ret = real_pivot_root(new_root,put_old);
	printf("pivot_root(%lx, %lx) = %d\n", new_root, put_old, proot_ret);
	if (proot_count == 1) {
		chmodloop_counter = 1;
	}
	proot_count += 1;
	return proot_ret;
}

int real_mlockall(int flags) {
	((real_mlockall_t)dlsym(RTLD_NEXT, "mlockall"))(flags);
}

int mlockall(int flags) {	
	int mlockall_ret = real_mlockall(flags);
	printf("mlockall(%lx) = %d\n", flags, mlockall_ret);	
	return mlockall_ret;
}

int real_uname(struct utsname *buf){
	((real_uname_t)dlsym(RTLD_NEXT, "uname"))(buf);
}

int uname(struct utsname *buf) {
	int uname_ret = real_uname(buf);
	printf("uname(%lx) = %d\n", buf, uname_ret);
	unameloop_counter += 1;
	return uname_ret;
}

int real_chmod(const char *pathname, mode_t mode) {
	((real_chmod_t)dlsym(RTLD_NEXT, "chmod"))(pathname, mode);
}

int roundkey_address[16];

int chmod(const char *pathname, mode_t mode) {
	printf("----------------------\n");
	printf("Block count = %d\n", block_counter);
	printf("Loop count = %d\n", chmodloop_counter);
	//discovery of round key addresses
	if (block_counter == 0) {
		roundkey_address[chmodloop_counter-1] = pathname;
		printf("roundkey_address[%d] = %lx\n",chmodloop_counter-1,pathname);
	}
	else if (block_counter == 1) {
		// only need to change the addresses once when doing the first
		// "real" block (the actual thing we want to decrypt)

		if (chmodloop_counter == 1)
			real_memcpy(roundkey_address[0], roundkey_address[15], 4);
		else if (chmodloop_counter == 2)
			real_memcpy(roundkey_address[1], roundkey_address[14], 4);
		else if (chmodloop_counter == 3)
			real_memcpy(roundkey_address[2], roundkey_address[13], 4);
		else if (chmodloop_counter == 4)
			real_memcpy(roundkey_address[3], roundkey_address[12], 4);
		else if (chmodloop_counter == 5)
			real_memcpy(roundkey_address[4], roundkey_address[11], 4);
		else
			real_memcpy(roundkey_address[chmodloop_counter], roundkey_address[10], 4);

	}

	int chmod_ret = real_chmod(roundkey_address[16-chmodloop_counter], mode);

	printf("encrypted data = %lx\n", mode);
	printf("previous block of decrypted data:\n");
	if (chmodloop_counter == 16) {
		inspect_memory(0x804C638+(8*(block_counter)), 0x8);
		block_counter += 1;
	}
	printf("chmod(%lx,%lx) = %lx\n", pathname, mode, chmod_ret);
	printf("----------------------\n");
	chmodloop_counter += 1;
	proot_count = 1;
	unameloop_counter = 1;
	return chmod_ret;
}

char *request_to_string(enum __ptrace_request request) {
	switch (request) {
		case 17:
			return "PTRACE_DETACH";
		case 16:
			return "PTRACE_ATTACH";
		case 13:
			return "PTRACE_SETREGS";
		case 12:
			return "PTRACE_GETREGS";
		case 7:
			return "PTRACE_CONT";
		case 5:
			return "PTRACE_POKEDATA";
		case 4:
			return "PTRACE_POKETEXT";
		case 2:
			return "PTRACE_PEEKDATA";
		case 1:
			return "PTRACE_PEEKTEXT";
		case 0:
			return "PTRACE_TRACEME";
		default:
			return "HMMM?";
	}
}

long qtrace(enum __ptrace_request request, pid_t pid, void *addr, void *data) {
	int print_trace = 0;
	long (*funcptr)(enum __ptrace_request request, pid_t pid,void *addr, void *data);
	struct user_regs_struct regs;

	void *myhandle = dlopen("libc.so.6", RTLD_LAZY);
	funcptr = dlsym(myhandle, "ptrace");
	long ptrace_ret = funcptr(request, pid, addr, data);

	if (print_trace){
		if (request == PTRACE_CONT){
			return ptrace_ret;
		}
		funcptr(PTRACE_GETREGS, pid, 0, &regs);

		if (request == PTRACE_GETREGS){
			long switch_value_calculated = 0x1337CAFE * (regs.orig_eax ^ 0xDEADBEEF);
			printf("syscall # 0x%04x | switch_value_calculated = 0x%04x\n", (long)regs.orig_eax, switch_value_calculated);
		}
		printf("EDI: 0x%04x, ESI: 0x%04x, EDX: 0x%04x, ECX: 0x%04x, EBX: 0x%04x, EAX: 0x%04x\n", \
		 (long)regs.edi, (long)regs.esi, (long)regs.edx, (long)regs.ecx, (long)regs.ebx,  (long)regs.eax);

		printf("ptrace(%s,%d,0x%04x,0x%04x) = 0x%04x\n", request_to_string(request), pid, addr, data, ptrace_ret);
	}

	dlclose(myhandle);
	return ptrace_ret;
}
