import javax.swing.JOptionPane;

public class JOptionPaneTest {
    public static void main(String[] args) {
        String x = JOptionPane.showInputDialog(null, "x = ", "Input", JOptionPane.QUESTION_MESSAGE);
        String y = JOptionPane.showInputDialog(null, "y = ", "Input", JOptionPane.QUESTION_MESSAGE);
        JOptionPane.showMessageDialog(null, "Sum = " + (Integer.valueOf(x) + Integer.valueOf(y)), "Output", JOptionPane.INFORMATION_MESSAGE);
    }
}