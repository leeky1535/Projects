package hw1;

import java.util.ArrayList;

abstract class Bank {
	public String name = "Chase Bank";
	static ArrayList<User> Users = new ArrayList<User>();
	static ArrayList<Branch> Branches = new ArrayList<Branch>();
	
	public int findUser(int account_number) {
		for(int i = 0; i < Users.size(); i++) {
			if(Users.get(i).account.account_number == account_number) {
				return i;
			}
		}
		return -1;
	}

}