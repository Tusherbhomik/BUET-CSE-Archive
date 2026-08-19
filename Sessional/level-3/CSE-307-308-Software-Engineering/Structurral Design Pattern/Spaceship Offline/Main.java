import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner=new Scanner(System.in);
        SpaceShipController operator=null;

        while (scanner.hasNextLine())
        {
            String input = scanner.nextLine();
            String[] commands = input.split(" ");
            switch (commands[0]) {
                case "login" -> {
                    String trimmedInput = commands[1].substring(0, commands[1].length() - 1);
                    boolean isCrewMate=trimmedInput.equalsIgnoreCase("crew");

                    if(isCrewMate){
                        operator=new CrewMate();


                    }else{
                        operator=new ImposterDecorator(new CrewMate());

                    }
                    operator.login();
                }
                case "work" -> {
                    assert operator != null;
                    operator.work();
                }
                case "repair" -> {
                    assert operator != null;
                    operator.repair();
                }
                case "logout" -> {
                    assert operator != null;
                    operator.logout();
                }
                case "exit"  -> System.exit(0);
                default -> System.out.println("Invalid Commands");
            }
        }
        scanner.close();
    }
}
