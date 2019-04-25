package hw1;

import java.io.FileWriter;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.StringTokenizer;
public class Branch extends Bank{
	String Branchname;
	Scanner number = new Scanner(System.in);
	Scanner user_input = new Scanner(System.in);
	
	int age, amount, salary;
	int choice;
	boolean validity;

	
	Branch(){}
	Branch(String branchname){
		this.Branchname = branchname;
		super.Branches.add(this);
	}
	
	protected void createAccount(String name, int age, String address, int salary, String account_type, int beginning_deposit) {
		if (checkAccountValidity(age, salary, account_type)){
			User newuser = new User(name, age, address, salary, account_type, this.Branchname, beginning_deposit);
			super.Users.add(newuser);
		}
		
	}
	public int checkAgeValid(String age_str) throws AgeNotValid {
		int age = isNumber(age_str);
        if (age<18) {
            throw new AgeNotValid("Exception: Age must be greater than 18");
        }

        return age;
    }
	
	protected void createAccount2(){
		int age, amount, salary;
		String Username, account_type, amount_str, age_str, email, salary_str, address;
		do{
			System.out.println("What is your name?\n");
			Username = user_input.nextLine();
		} while(Username.isEmpty());
ageloop:while (true) {
			System.out.println("How old are you? (integer greater than 18)\n");
			age_str = user_input.nextLine();
			try {
				age = checkAgeValid(age_str);
				break ageloop;
			}
			catch (AgeNotValid anv) {
				System.out.println(anv);
			}
			
		}
		do{
			System.out.println("What is your address?\n");
			address = user_input.nextLine();
		} while(address.isEmpty());
		StringTokenizer str = new StringTokenizer(address, " ,:;?!'");
		while(str.hasMoreTokens()) {
			System.out.print(str.nextToken() + "; ");
		}
		System.out.print("\n");
		do{
			System.out.println("Your salary per year? (only integer)\n");
			salary_str = user_input.nextLine();
			salary = isNumber(salary_str);
		} while(age_str.isEmpty() || salary < 0);
		do{
			System.out.println("What is your E-mail?\n");
			email = user_input.nextLine();
			validity = isEmailValid(email);
		} while(Username.isEmpty() || validity == false);
		do{
			System.out.println("Account type? (saving or current)\n");
			account_type = user_input.nextLine();
		} while(account_type.isEmpty() || (!account_type.equalsIgnoreCase("saving") && !(account_type.equalsIgnoreCase("current"))));
		do{
			System.out.println("How much initial deposit?\n");
			amount_str = user_input.nextLine();
			amount = isNumber(amount_str);
		} while(amount_str.isEmpty() || amount < 0);
		
		if (checkAccountValidity(age, salary, account_type)){
			User newuser = new User(Username, age, address, salary, account_type, this.Branchname, amount);
			super.Users.add(newuser);
			try {
				FileWriter  fw=new FileWriter("ChaseBankUsersInfo.txt", true);
				fw.write(newuser.toString());
				fw.close();
			}
			catch (Exception e) {
				System.err.println(e);
			}
			System.out.println(newuser.toString());
		}
		else System.out.println("Cannot make an account");
	}
	
	
	public void depositMoney(int account_number, int money) {
		int index = findUser(account_number);
		if(index >= 0) {
			Users.get(index).account.balance += money;
		}
		else {
			System.out.println("Can't find the account\n");
		}
		System.out.println(Users.get(index));
	}
	
	public int checkBalance(int balance, int withdraw_amount) throws BalanceNotEnough {
        if ((balance-withdraw_amount)<0) {
            throw new BalanceNotEnough("Exception: Your Balance is not enough");
        }

        return (balance-withdraw_amount);
    }
	public void withdrawMoney(int account_number, int money) {
		int index = findUser(account_number);
		if(index >= 0) {
			try{
				Users.get(index).account.balance = checkBalance(Users.get(index).account.balance, money);
			}
			catch (BalanceNotEnough bne) {
				System.out.println(bne);
			}
		}
		else {
			System.out.println("Can't find the account\n");
		}
		System.out.println(Users.get(index));
	}
	
	private boolean checkAccountValidity(int age, int salary, String account_type){
		if((age < 18 || salary < 20000 || salary > 50000) && account_type.equalsIgnoreCase("saving")) {
			System.out.println("Can't make account\n");
			return false;
		}
		else if((age < 25 || salary < 100000) && account_type.equalsIgnoreCase("current")) {
			System.out.println("can't make account\n");
			return false;
		}
		else return true;
	}
	
	
	public static int isNumber(String str) {
	    try {
	        int v = Integer.parseInt(str);
	        return v;
	    } catch (NumberFormatException nfe) {
	    }
	    return -1;
	}
	
	public static boolean isEmailValid(String email) {
	    final Pattern EMAIL_REGEX = Pattern.compile("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?", Pattern.CASE_INSENSITIVE);
	    return EMAIL_REGEX.matcher(email).matches();
	}
	@Override
	public String toString() {
		return Branchname;
	}
	
	


}

