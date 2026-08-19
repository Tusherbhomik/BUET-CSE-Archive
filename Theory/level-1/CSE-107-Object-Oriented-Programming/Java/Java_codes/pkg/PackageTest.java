package pkg;

import pkg.subpkg.Pakage;
//import pkg.subpkg.PPP; // not found
import pkg.Pkg;
import pkg.PP;

public class PackageTest {
    public static void main(String[] args) {
        Pakage p = new Pakage();   
        // no method of p is visible
        Pkg pk = new Pkg();
        Pkg.print();
    }
}