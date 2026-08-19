import java.util.ArrayList;

public class Bank {
    private  double yearlyMaintainFee;
    private ArrayList<Employee> employees;
    private ArrayList<Account> accounts;

    private ArrayList<Loan> loansList;

    private int noOfOfficer;
    private int noOfCashier;
    private double internalFund;
    private double loanInterestRate;

    public Bank(int noOfOfficer, int noOfCashier, double internalFund) {
        this.noOfOfficer = noOfOfficer;
        this.noOfCashier = noOfCashier;
        this.internalFund = internalFund;
        this.loanInterestRate = 10;
        this.yearlyMaintainFee=500;


        employees = new ArrayList<>();
        employees.add(new ManagingDirector("MD"));
        accounts = new ArrayList<>();
        loansList = new ArrayList<Loan>();

        System.out.print("Bank Created; MD,");
        //Officer created
        for (int i = 0; i < noOfOfficer; i++) {
            employees.add(new Officer("S" + (i+1)));
            System.out.print("S" + (i + 1) + ",");
        }


        for (int i = 0; i < noOfCashier; i++) {
            employees.add(new Cashier("C" + (i+1)));
            System.out.print("C" + (i + 1) + ",");
        }
        System.out.println("Created");
    }


    private boolean accountExist(String name) {
        for (Account account : accounts) {
            if (account.getAccountHolderName().equalsIgnoreCase(name)) {
                return true;
            }
        }
        return false;
    }

    public void createAccount(String accountHolderName, String accountType, double initDeposit) {

        if (!accountExist(accountHolderName)) {
            if (accountType.equalsIgnoreCase("Student")) {
                Account student = new Student(accountHolderName, initDeposit);
                System.out.println("Student account for " + accountHolderName + " Created; initial balance " + initDeposit + "$");
                accounts.add(student);//why here is error? Null pointer exception
                internalFund += initDeposit;
            } else if (accountType.equalsIgnoreCase("Savings")) {
                Account savings = new Savings(accountHolderName, initDeposit);
                System.out.println("Savings account for " + accountHolderName + " Created; initial balance " + initDeposit + "$");
                accounts.add(savings);//why here is error? Null pointer exception
                internalFund += initDeposit;
            } else if (accountType.equalsIgnoreCase("FixedDeposit")) {

                if (initDeposit < 100000) {
                    System.out.println("Amount at least 100,000$ ");
                } else {
                    Account fixedDeposit = new FixedDeposit(accountHolderName, initDeposit);
                    System.out.println("Fixed Deposit  account for " + accountHolderName + " Created; initial balance " + initDeposit + "$");
                    accounts.add(fixedDeposit);
                    System.out.println(fixedDeposit.getAccountHolderName());
                    System.out.println(accounts.size());
                    internalFund += initDeposit;
                }
            }
            else{
                System.out.println("Wrong Account name provided");
            }

        } else {
            System.out.println("Already had an account with the same name");
        }
    }

    public void depositAccount(String accountHolderName, double amount) {
        if (accountExist(accountHolderName)) {
            for (Account account : accounts) {
                if (account.getAccountHolderName().equalsIgnoreCase(accountHolderName) && (!(account instanceof FixedDeposit))) {
                    account.deposit(amount);
                    System.out.println(amount + "$ deposited; " + "Current Balance " + account.getCurrentBalance() + "$");
                }
                else if(account instanceof FixedDeposit){
                    if(amount< ((FixedDeposit) account).getLowestDeposit())
                    {
                        System.out.println("Minimum amount you can deposit"+ ((FixedDeposit) account).getLowestDeposit()+"$");
                    }
                    else {
                        account.deposit(amount);
                        System.out.print(amount + "$ deposited; " + "Current Balance " + account.getCurrentBalance() + "$");
                    }

                }
            }
            internalFund += amount;
        } else {
            System.out.println("You are not allow to Deposit");
        }
    }

    public void withDrawFromAccount(String accountHolderName, double amount) {
        if (accountExist(accountHolderName)) {
            for (Account account : accounts) {
                if (account.getAccountHolderName().equalsIgnoreCase(accountHolderName) && account.canTheAmountWithdraw(amount)) {
                    account.withDraw(amount);
                    System.out.println(amount + " amount withdrawn; current balance " + account.getCurrentBalance());
                } else {
                    System.out.println("Invalid transaction; current balance " + account.getCurrentBalance());
                }
            }
            internalFund += amount;
        } else {
            System.out.println("You are not allowed to Withdraw");
        }

    }

    public void query(String accountHolderName) {
        for (Account account : accounts) {
            if (account.getAccountHolderName().equalsIgnoreCase(accountHolderName)) {
                System.out.print("Current Balance " + account.getCurrentBalance() + "$");
                if (account.getPendingLoanAmount() > 0) {
                    System.out.print(" ,loan " + 500 + "$");
                }
                System.out.println();
            }

        }
    }

    //jeta loan shetake vabtesi oita she tule feltse
    //loan request korle approve kora por internal fund will change
    public void requestLoan(String accountHolderName, double amount) {
        if (accountExist(accountHolderName)) {
            for (Account account : accounts) {
                if (account.getAccountHolderName().equalsIgnoreCase(accountHolderName)) {
                    Loan loan = new Loan(account, amount);

                    loansList.add(loan);
                    System.out.println("Loan request successful, sent for approval");
                }
            }

        } else {
            System.out.println("You are not allowed to request loan");
        }
    }

    public void open(String accountName)//access dibo ekhane
    {
        for (Account account : accounts) {
            if (account.getAccountHolderName().equalsIgnoreCase(accountName)) {
                System.out.println("Welcome back, " + accountName);
            }
        }
        for (Employee employee : employees) {
            if (employee.getName().equalsIgnoreCase(accountName) && (employee instanceof ManagingDirector || employee instanceof Officer)) {
                System.out.println(accountName + " active");
                for (Loan loan : loansList) {
                    if (!loan.isApproved()) {
                        System.out.println(", there are loan approval pending");
                        break;
                    }
                }
            }
        }
    }

    private boolean canHeApproveLoanFunc(String name) {
        for (Employee employee : employees) {
            if (employee.getName().equalsIgnoreCase(name) && (!(employee instanceof Cashier))) {
                return true;
            }
        }
        return false;
    }


    public void approveLoan(String name) {
        if (canHeApproveLoanFunc(name)) {//ican add is he md here too
            for (Loan loan : loansList) {
                if (!loan.isApproved()) {
                    loan.approveLoan();
                    internalFund -= loan.getLoanAmount();//loan approve korar por internal fund change hocche
                    System.out.println("Loan for " + loan.getAccount().getAccountHolderName() + " approved");
                }
            }
        }

    }

    private boolean isHeMD(String name) {
        if (name.equalsIgnoreCase("MD"))
            return true;
        return false;
    }

    private boolean canHeChangeInterestRate(String name) {
        for (Employee employee : employees) {
            if (employee.getName().equalsIgnoreCase(name) && employee instanceof ManagingDirector)
                return true;
        }
        return false;

    }


    public void change(String user, String accountType, double newInterestRate) {
        if (canHeChangeInterestRate(user)) {
            for (Account account : accounts) {
                if (accountType.equalsIgnoreCase("Student") && account instanceof Student) {
                    account.setInterestRate(newInterestRate);
                } else if (accountType.equalsIgnoreCase("FixedDeposit") && account instanceof FixedDeposit) {
                    account.setInterestRate(newInterestRate);
                } else if (accountType.equalsIgnoreCase("Savings") && account instanceof Savings) {
                    account.setInterestRate(newInterestRate);
                }
                System.out.println("Interest Rate changes to " + newInterestRate + "% for " + accountType);

            }
        } else {
            System.out.println("You don’t have permission for this operation");
        }

    }

    public void close(String name) {
        //System.out.println("will close "+name);
        for (Account account : accounts) {
            //System.out.println(account.getAccountHolderName());
            if (account.getAccountHolderName().equalsIgnoreCase(name)) {
                System.out.println("Transaction for " + name + " closed");
                break;
            }
        }
        for (Employee employee : employees) {
            if (employee.getName().equalsIgnoreCase(name)) {
                System.out.println("Operations for " + name + " closed");
                break;
            }
        }
    }

    private boolean isHeCanLookUp(String admin) {
        for (Employee employee : employees) {
            if (employee.getName().equalsIgnoreCase(admin)) {
                return true;
            }
        }
        return false;

    }


    public void lookUp(String admin, String customer) {
        if (isHeCanLookUp(admin)) {
            for (Account account : accounts) {
                if (account.getAccountHolderName().equalsIgnoreCase(customer)) {
                    System.out.println(customer + "'s current balance " + account.getCurrentBalance());

                }
            }
        }
    }

    public void seeInternalFund(String name) {
        if (isHeMD(name)) {
            System.out.println("Current Fund is " + internalFund + "$");
        } else {
            System.out.println("You don’t have permission for this operation");
        }
    }

    public void INC() {

        for (Account account : accounts) {
            if (!(account instanceof Student)) {
                account.setCurrentBalance((account.getCurrentBalance() - yearlyMaintainFee));
            }
            if(account instanceof FixedDeposit)
            {
                ((FixedDeposit) account).setAgeIncrement();
            }
            account.setCurrentBalance(((account.getCurrentBalance() * account.getInterestRate()) / 100) + account.getCurrentBalance());
            account.setCurrentBalance(account.getCurrentBalance() - ((account.getPendingLoanAmount() * loanInterestRate) / 100));
        }
        System.out.println("1 year passed");
    }

    //subtract 500 before adding interest
//    public void seeAccounts()
//    {
//        for(Account account:accounts)
//        {
//            System.out.println(account.getAccountHolderName()+" "+account.getCurrentBalance());
//        }
//        for(Employee employee:employees)
//        {
//            System.out.println(employee.getName());
//        }
//    }
}

