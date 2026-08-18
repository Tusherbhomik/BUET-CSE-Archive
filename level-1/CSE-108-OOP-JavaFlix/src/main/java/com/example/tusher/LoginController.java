package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.List;
public class LoginController {
    @FXML
    public static SocketWrapper socketWrapper;
    private  static String companyName;
    @FXML
    private TextField clientNameTextField;
    public static List<Movie>myMovieList;
    public void onLoginClick(ActionEvent actionEvent) throws IOException, ClassNotFoundException {



        companyName= clientNameTextField.getText();
        System.out.println(companyName);
        Client c=new Client(companyName);
        Client.productionCompany=companyName;//for further use while printing in table
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("menu-scene.fxml"));
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle(companyName);
        stage.setScene(scene);
        }
    public void onExitClick(ActionEvent actionEvent) {
        System.exit(0);
    }
}
