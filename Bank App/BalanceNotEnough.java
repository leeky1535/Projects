package hw1;
import java.lang.Exception;

public class BalanceNotEnough extends Exception{
	BalanceNotEnough(String str){
		super(str);
	}
}

