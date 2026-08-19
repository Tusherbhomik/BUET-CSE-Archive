public class ImposterDecorator extends CrewMateDecorator{

    public ImposterDecorator(SpaceShipController spaceShipController)
    {
        super(spaceShipController);
    }

    public void login() {
        decoratedCrew.login();
        extraLoginInfo();
    }
    public void repair() {
        decoratedCrew.repair();
        extraRepairInfo();

    }
    public void work() {
        decoratedCrew.work();
        extraWorkInfo();
    }
    public void logout() {
        decoratedCrew.logout();
        extraLogout();
    }
    private void extraLoginInfo()
    {
        System.out.println("We won’t tell anyone; you are an imposter.");
    }
    private void extraRepairInfo()
    {
        System.out.println("Damaging the spaceship.");
    }
    private void extraWorkInfo()
    {
        System.out.println("Trying to kill a crewmate.");
        System.out.println("Successfully killed a crewmate.");
    }
    private void extraLogout()
    {
        System.out.println("See you again Comrade ImposterDecorator");
    }
}
