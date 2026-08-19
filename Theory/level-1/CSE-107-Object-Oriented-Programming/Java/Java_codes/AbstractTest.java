abstract class S {
    abstract void call();
    void call2() {
        print.string("hello from call2");
    }
}
class T extends S {
    void call() {
        print.string("call from call");
    }
}

abstract class Timer {
    long start,end;
    abstract void Start();
    abstract void Stop();
    long Seconds() {
        return -(start - end) / 1000;
    }
}
class X extends Timer {
    void Start() {
       start = System.currentTimeMillis(); 
    }
    void Stop() {
        end = System.currentTimeMillis();
    }
    long Seconds() {
        return super.Seconds();
    }
}
public class AbstractTest {
    public static void main(String[] args) throws Exception {
        X x = new X();
        x.Start();
        Thread.sleep(2000);
        x.Stop();
        print.string(x.Seconds() + "");
        S s = new S() {
            void call() {
                print.string("call method of an abstract class");
            }
        };
        s.call();
    }    
}