public abstract class CrewMateDecorator implements SpaceShipController {

    protected SpaceShipController decoratedCrew;
    public CrewMateDecorator(SpaceShipController spaceShipController)
    {
        this.decoratedCrew=spaceShipController;
    }
    public void login()
    {
        decoratedCrew.login();
    }
    public  void repair()
    {
        decoratedCrew.repair();
    }
    public void work()
    {
        decoratedCrew.work();
    }
    public void logout()
    {
        decoratedCrew.logout();

    }
}
