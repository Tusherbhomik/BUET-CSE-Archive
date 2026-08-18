public class Loan {

    Account account;
    double loanAmount;
    boolean approved;

    public Account getAccount() {
        return account;
    }

    public double getLoanAmount() {
        return loanAmount;
    }

    Loan(Account account, double amount) {
        this.account = account;
        this.loanAmount = amount;
        approved = false;
    }

    public boolean isApproved() {
        return approved;
    }

    // The maximum allowable loan for savings,
    //student, and fixed deposit accounts are 10,000$, 1,000$, 100,000$, respectively. All loans
    //have a fixed 10% interest rate, which will be deducted after one year. The loans must be
    //approved by an employee of the bank.
    public void approveLoan() {
        if (account.canApproveLoan(loanAmount)) {
            account.setLoanAmount(loanAmount);
            account.deposit(loanAmount);
        }
        approved = true;
    }
}
