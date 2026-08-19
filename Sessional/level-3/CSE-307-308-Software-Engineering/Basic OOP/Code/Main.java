import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        Bank bangladeshBank = new Bank(2, 5, 1000000);

        Scanner scanner = new Scanner(System.in);
        String currentUser = "";

        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            if (line.equals("exit")) {
                System.exit(0);
            }

            String[] words = line.split(" ");

            if (words[0].equalsIgnoreCase("Create")&&currentUser.equalsIgnoreCase("")) {
                bangladeshBank.createAccount(words[1], words[2], Utility.stringToNumber(words[3]));
                currentUser = words[1];
                //System.out.println("User :" + currentUser);

            } else if (words[0].equalsIgnoreCase("Deposit")) {
                bangladeshBank.depositAccount(currentUser, Utility.stringToNumber(words[1]));
                //System.out.println("User :" + currentUser);
            } else if (words[0].equalsIgnoreCase("Withdraw")) {
                bangladeshBank.withDrawFromAccount(currentUser, Utility.stringToNumber(words[1]));
                //System.out.println("User :" + currentUser);
            } else if (words[0].equalsIgnoreCase("Query")) {

                bangladeshBank.query(currentUser);
                //System.out.println("User :" + currentUser);
            }
            //current user type ki  pass kora lage ???
            else if (words[0].equalsIgnoreCase("Request")) {
                    bangladeshBank.requestLoan(currentUser,  Utility.stringToNumber(words[1]));
                    //System.out.println("User :" + currentUser);
            } else if (words[0].equalsIgnoreCase("Close")) {
                bangladeshBank.close(currentUser);
                currentUser = "";
                //System.out.println("User :" + currentUser);

            } else if (words[0].equalsIgnoreCase("Open")&&currentUser.equalsIgnoreCase("")) {

                bangladeshBank.open(words[1]);
                currentUser = words[1];
                //System.out.println("User :" + currentUser);

            } else if (words[0].equalsIgnoreCase("Approve")) {

                bangladeshBank.approveLoan(currentUser);
                //System.out.println("User :" + currentUser);
            } else if (words[0].equalsIgnoreCase("Change")) {

                bangladeshBank.change(currentUser, words[1], Utility.stringToNumber(words[2]));
                //System.out.println("User :" + currentUser);

            } else if (words[0].equalsIgnoreCase("Lookup")) {
                bangladeshBank.lookUp(currentUser, words[1]);
                //System.out.println("User :" + currentUser);

            } else if (words[0].equalsIgnoreCase("See")) {
                //System.out.println("User :" + currentUser);
                bangladeshBank.seeInternalFund(currentUser);
            }
            else if(words[0].equalsIgnoreCase("INC"))
            {
                bangladeshBank.INC();
            }
//            else if(words[0].equalsIgnoreCase("show"))
//            {
//                bangladeshBank.seeAccounts();
//            }
            else {
                System.out.println("Enter a valid Operation");
            }

        }

    }
}
