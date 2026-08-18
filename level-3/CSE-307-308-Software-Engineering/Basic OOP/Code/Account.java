import java.util.Scanner;

public interface Account {


    String getAccountHolderName();
    void setCurrentBalance(double amount);

    double getCurrentBalance();

    double getPendingLoanAmount();

    void setLoanAmount(double amount);

    void deposit(double depositAmount);

    void withDraw(double withDrawAmount);

    double getInterestRate();

    void setInterestRate(double newInterestRate);

    boolean canApproveLoan(double amount);

    boolean canTheAmountWithdraw(double amount);


}
