package hw1;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.lang.Exception;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;

public class BankApp{
	static {
		Branch St_Louis = new Branch("St.Louis");
		Branch Chicago = new Branch("Chicago");
		/*St_Louis.createAccount("Jae",25, "106 E Healey St 13, Champaign, IL, 61820", 100000,"current", 4000);
		St_Louis.createAccount("Kyung", 18, "3322 Bradly Ave, IL, 33333", 22222, "saving", 10000);
		St_Louis.createAccount("Lee", 17, "1155 E Dumb St 22, St.Louis, IL, 61820", 56222, "saving", 56000);
		Chicago.createAccount("Paul", 44, "23 E Malberry Ave, Champaign, IL, 61820", 120000, "current", 10000);
		System.out.println(Bank.Branches);
		System.out.println(Bank.Users);*/
		try {
			FileWriter  fw=new FileWriter("ChaseBankUsersInfo.txt");
			for(int i = 0; i < Bank.Users.size(); i++) {
				fw.write(Bank.Users.get(i).toString());
			}
			fw.close();
		}
		catch (Exception e) {
			System.err.println(e);
		}
	}

	public static void main(String[] args) {
		Scanner number = new Scanner(System.in);
		Scanner user_input = new Scanner(System.in);
		String branch;
		int age, amount;
		int choice;
		Branch curr_branch = null;
		int deposit, account_number, index, withdraw;
		String deposit_str, account_number_str, withdraw_str;
		
loop:	while(true){
			System.out.print("1: Create a Branch of a Bank\n2: Make an account\n3: Deposit\n4: Withdraw\n5: See All Users\n6: See all Branches\n7: Done\n");
			do {
				choice = isPositiveInteger(number.nextLine());
			} while(choice < 0 || choice > 7);
			switch (choice) {
				case 1:					
					while(true) {
						System.out.println("Provide a name for the branch\n");
						branch = user_input.nextLine();
						if(branch.isEmpty() == true) continue;
						if(findBranch(branch) != -1) {
							System.out.println("A branch with same name already exist");
							continue;
						}
						Branch new_branch= new Branch(branch);
						break;
					}
					
					break;
				case 2:
					int found = 0;
					do {
						while(true) {
							System.out.println("Provide a name for the branch " + Bank.Branches + "\n");
							branch = user_input.nextLine();
							if(branch.isEmpty() || (index = findBranch(branch)) == -1) {
								System.out.println("No branch found\n");
								continue;
							}
							break;
						}
						curr_branch = Bank.Branches.get(index);
						curr_branch.createAccount2();
						found = 1;
					} while(found == 0);
					break;
				case 3:
					do{
						System.out.println("What's your account number");
						account_number_str = user_input.nextLine();
						account_number = isPositiveInteger(account_number_str);
					} while(account_number_str.isEmpty() || account_number >= Bank.Users.size());
					do{
						System.out.println("How much do you want to deposit today?\n");
						deposit_str = user_input.nextLine();
						deposit = isPositiveInteger(deposit_str);
					} while(deposit_str.isEmpty() || deposit <= 0);
					while(true) {
						System.out.println("Provide a name for the branch " + Bank.Branches + "\n");
						branch = user_input.nextLine();
						if(branch.isEmpty() || (index = findBranch(branch)) == -1) {
							System.out.println("No branch found\n");
							continue;
						}
						break;
					}
					Bank.Branches.get(index).depositMoney(account_number, deposit);
					break;
				case 4:
					do{
						System.out.println("What's your account number");
						account_number_str = user_input.nextLine();
						account_number = isPositiveInteger(account_number_str);
					} while(account_number_str.isEmpty() || account_number >= Bank.Users.size());
					do{
						System.out.println("How much do you want to withdraw today?\n");
						withdraw_str = user_input.nextLine();
						withdraw = isPositiveInteger(withdraw_str);
					} while(withdraw_str.isEmpty() || withdraw <= 0);
					while(true) {
						System.out.println("Provide a name for the branch " + Bank.Branches + "\n");
						branch = user_input.nextLine();
						if(branch.isEmpty() || (index = findBranch(branch)) == -1) {
							System.out.println("No branch found\n");
							continue;
						}
						break;
					}
					Bank.Branches.get(index).withdrawMoney(account_number, withdraw);
					break;
				case 5:
					System.out.println(Bank.Users);
					break;
				case 6:
					System.out.println(Bank.Branches);
					break;
				case 7:
					break loop;
				default:
					break;
			}
		}
		System.out.print("Done");
		number.close();
		user_input.close();

	}
	public static int isPositiveInteger(String str) {
	    try {
	        int v = Integer.parseInt(str);
	        return v;
	    } catch (NumberFormatException nfe) {
	    	return -1;
	    }
	}
	
	public static int findBranch(String branch_name) {
		for(int i = 0; i < Bank.Branches.size(); i++) {
			if(Bank.Branches.get(i).Branchname.equalsIgnoreCase(branch_name)) {
				return i;
			}
		}
		return -1;
	}

}

