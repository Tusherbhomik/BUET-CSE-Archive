
public class Student implements Account {

    private String accountHolderName;
    private double initialDeposit;
    private double currentBalance;
    private double interestRate;
    private double pendingLoan;

    public Student(String accountHolderName, double initialDeposit) {
        this.accountHolderName = accountHolderName;
        this.initialDeposit = initialDeposit;
        this.currentBalance = initialDeposit;
        this.interestRate = 5;
        pendingLoan = 0;
    }

    @Override
    public String getAccountHolderName() {
        return accountHolderName;
    }

    @Override
    public void setCurrentBalance(double amount ) {
        currentBalance=amount;
    }


    @Override
    public double getCurrentBalance() {
        return currentBalance;
    }

    @Override
    public double getPendingLoanAmount() {
        return pendingLoan;
    }

    @Override
    public void setLoanAmount(double amount) {
        pendingLoan += amount;
    }


    @Override
    public void deposit(double depositAmount) {
        currentBalance += depositAmount;
    }

    @Override
    public boolean canTheAmountWithdraw(double withDrawAmount) {
        return !(withDrawAmount > 10000 || currentBalance < withDrawAmount);
    }

    @Override
    public void withDraw(double withDrawAmount) {
        currentBalance -= withDrawAmount;
    }

    @Override
    public double getInterestRate() {
        return interestRate;
    }

    @Override
    public void setInterestRate(double newInterestRate) {
        interestRate = newInterestRate;
    }

    @Override
    public boolean canApproveLoan(double amount) {
        return !(amount > 1000);
    }
}
