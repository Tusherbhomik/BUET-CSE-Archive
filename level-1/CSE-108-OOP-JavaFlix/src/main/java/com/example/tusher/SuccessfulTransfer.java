package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class SuccessfulTransfer {

    public void onBackToMainMenuClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("menu-scene.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Menu Scene");
        stage.setScene(scene);
    }
}
