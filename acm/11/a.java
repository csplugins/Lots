import java.math.*;
import java.util.*;

public class a{
  public static void main(String[] args){
    Scanner reader = new Scanner(System.in);
    BigInteger a = new BigInteger(reader.next());
    BigInteger b = new BigInteger(reader.next());
    BigInteger c = new BigInteger("0");
    while(b.compareTo(c) == 1){
      BigInteger temp;
      temp = a;
      a = b;
      b = temp.mod(b);
    }

    System.out.println(a);
  }
}
