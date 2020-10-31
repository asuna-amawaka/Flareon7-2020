 //written by Asuna Amawaka for FlareOn7 10_-_break

import java.math.BigInteger; 
import java.util.Scanner;
import java.util.Random;
  
public class Solver 
{ 
    private static String convertString(String hexString) {

        StringBuilder output = new StringBuilder();
        int l = hexString.length();
        for(int i = l; i >= 2; i -= 2) {
            String str = hexString.substring(i-2, i);
            output.append((char)Integer.parseInt(str, 16));
        }
        return output.toString();
    }
    
    public static void main(String args[]) throws Exception 
    { 
        
        String q_ = "d1cc3447d5a9e1e6adae92faaea8770db1fab16b1568ea13c3715f2aeba9d84f";
        String c_ = "d036c5d4e7eda23afceffbad4e087a48762840ebb18e3d51e4146f48c04697eb";
        String h_ = "c10357c7a53fa2f1ef4a5bf03a2d156039e7a57143000c8d8f45985aea41dd31";
        BigInteger q = new BigInteger(q_,16);
        BigInteger c = new BigInteger(c_,16);
        BigInteger h = new BigInteger(h_,16);
        BigInteger key = BigInteger.valueOf(1);
        BigInteger S, i_S, res_check, soln, soln_check;
        
        System.out.println("k = " + key.toString(16));
        System.out.println("c = " + c.toString(16));
        System.out.println("q = " + q.toString(16));
        System.out.println("h = " + h.toString(16));
        
        S = h.modPow(key, q);
        System.out.println("S = " + S.toString(16));
        
        i_S = S.modInverse(q);
        System.out.println("S^-1 = " + i_S.toString(16));
        
        res_check = S.multiply(i_S).mod(q);
        System.out.println("S * S^-1 mod q = " + res_check.toString(16)); 
        
        soln = i_S.multiply(c).mod(q);
        System.out.println("S^-1 * c mod q = " + soln.toString(16)); 
        
        soln_check = soln.multiply(S).mod(q);
        System.out.println("S * M mod q = " + soln_check.toString(16)); 
        if (soln_check.compareTo(c)==0) {
            System.out.println("Got it! " + convertString(soln.toString(16))); 
        }
        
    } 
} 