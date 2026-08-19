

public class FixedDeposit implements Account {


    private String accountHolderName;
    private double incrementAge;


    private double initialDeposit;
    private double currentBalance;

    private double interestRate;
    private double pendingLoan;

    public double getLowestDeposit() {
        return lowestDeposit;
    }

    public void setLowestDeposit(double lowestDeposit) {
        this.lowestDeposit = lowestDeposit;
    }

    private double lowestDeposit;

    public FixedDeposit(String accountHolderName, double initialDeposit) {
        this.accountHolderName = accountHolderName;
        this.initialDeposit = initialDeposit;
        this.currentBalance = initialDeposit;
        this.incrementAge = 0;
        this.interestRate = 15;
        lowestDeposit=50000;
        pendingLoan=0;
    }
    public void setAgeIncrement()
    {
        incrementAge++;
    }

    public double getIncrementAge() {
        return incrementAge;
    }

    @Override
    public String getAccountHolderName() {
        return accountHolderName;
    }

    @Override
    public void setCurrentBalance(double amount) {
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
        return !(amount > 100000);
    }

    @Override
    public boolean canTheAmountWithdraw(double amount) {
        return incrementAge >=1;
    }


}
