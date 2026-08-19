package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.IOException;
public class Transfer {

    public String transferredMovieName;
    public String transferredCompanyName;
    @FXML
     TextField name;
    @FXML
     TextField companyName;

    public void onConfirmTransferClick(ActionEvent actionEvent) throws IOException {
        transferredMovieName=name.getText();
        transferredCompanyName=companyName.getText();

        String s=transferredMovieName+","+transferredCompanyName+","+Client.productionCompany;
        System.out.println(s);
        Client.socketWrapper.write(s);

        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("successful-transfer.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Menu Scene");
        stage.setScene(scene);

    }

    public void onTransferBackClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("menu-scene.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Add Movie Scene");
        stage.setScene(scene);
    }
}
