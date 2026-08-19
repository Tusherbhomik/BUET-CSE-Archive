public class CrewMate implements SpaceShipController {
    public  CrewMate()
    {}
    @Override
    public void login() {
        //loggedInUser=name;
        System.out.println("Welcome Crewmate");
    }

    @Override
    public void repair() {
        System.out.println("Repairing the spaceship.");
    }
    @Override
    public void work() {
        System.out.println("Doing research.");
    }

    @Override
    public void logout() {
        System.out.println("Bye Bye Crewmate. ");
    }
}
