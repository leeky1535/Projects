package hw1;
import java.lang.Exception;
public class AgeNotValid extends Exception { 
    public AgeNotValid(String str) {
        super(str);
    }
}