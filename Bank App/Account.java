package hw1;

public class Account{
	String account_type;  //"saving" or "current"
	static int max_account_number = 0;
	int account_number;    //This equals to the current user's index in Users ArrayList
	int balance = 0;
	Account(){
	}
	Account(String account_type){
		this.account_type = account_type;
		this.account_number = max_account_number;
		max_account_number += 1;
	}
	@Override
	public String toString() {
		return "Account [account_type=" + account_type + ", account_number=" + account_number + ", balance=" + balance
				+ "]";
	}
	
}