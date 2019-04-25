package hw1;

public class User {
	String name; 
	int age;
	String address;
	int salary;
	Account account;
	String Branchname;
	
	User(){
	}
	User(String name, int age, String address, int salary, String account_str, String Branchname, int initial_deposit){
		this.name = name;
		this.age = age;
		this.address = address;
		this.salary = salary;
		this.account = new Account(account_str);
		this.Branchname = Branchname;
		this.account.balance = initial_deposit;
	}

	@Override
	public String toString() {
		return "[name=" + name + ", age=" + age + ", address=\"" + (address) + "\"" + ", salary=" + salary + ", account=" + account.toString() + ", Branchname="
				+ Branchname+ "]\n";
	}
	
	
}