class LinkedList<T> {
    T data;
    LinkedList<T> previous,next;
    static LinkedList head=null,tail=null;
    LinkedList(T data) {
        this.data = data;
        if(head == null) {
            head = this;
            tail = this;
        }
    }
    void push_back(T data) {
        LinkedList<T> t = new LinkedList<T>(data);
        t.previous = tail;
        tail.next = t;
        tail = t;
    }
    void push_front(T data) {
        LinkedList<T> t = new LinkedList<T>(data);
        t.next = head;
        head.previous = t;
        head = t;
    }
    void pop_back() {
        tail = tail.previous;
        tail.next = null;
    }
    void pop_front() {
        head = head.next;
        head.previous = null;
    }
}
interface iStack<T> {
    void push(T e);
    T pop();
    T top();
    boolean isEmpty();
}
interface iQueue<T> {
    void push(T e);
    T pop();
    T front();
    boolean isEmpty();
}
class Stack<T> implements iStack<T> {
    LinkedList<T> ll;
    Stack() {
        ll = new LinkedList(-1);
    }
    public void push(T e) {
        ll.push_back(e);
    }
    public T pop() {
        T e = (T) ll.tail.data;
        ll.pop_back();
        return e;
    }
    public T top() {
        T e = (T) ll.tail.data;
        return e;
    }
    public boolean isEmpty() {
        if(ll.head == ll.tail) return true;
        return false;
    }
}
public class GenStkQue {
    public static void main(String[] args) {
        Stack<Integer> st = new Stack();
        for(int i=0;i<10;i++) st.push(i);
        for(int i=0;i<10;i++) System.out.println(st.pop());  
    }
}
