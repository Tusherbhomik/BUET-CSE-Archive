package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class InfoSceneController {
    private Stage stage;
    private FXMLLoader fxmlLoader;
    private Scene scene;
    private long sum=0;
    private Parent root;
    @FXML
    public static Stage myStage;


    public void onSearchBYTitleClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("title.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Find Your Movie");
        stage.setScene(scene);
    }

    public void onSearchByReleaseYearClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("release-year.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Find your Movie");
        stage.setScene(scene);
    }

    public void onFindMostRecentMovies(ActionEvent actionEvent) throws IOException {
        List<Movie> matchedList=new ArrayList<>();
        Client.myMovieList.sort(new YearComparator());
        for (Movie movie : Client.myMovieList) {
            if (movie.getReleaseYear().equals(Client.myMovieList.get(0).getReleaseYear())) {
                matchedList.add(movie);
            }
        }
        TableController.matchedMovieList=matchedList;
        stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("table.fxml"));
        scene = new Scene(fxmlLoader.load());
        stage.setTitle("In Table View");
        stage.setScene(scene);

    }

    public void onFindMovieWithMaxRevenue(ActionEvent actionEvent) throws IOException {
        List<Movie> updateList=new ArrayList<>();
        Client.myMovieList.sort(new RevenueComparator());
        for (Movie movie : Client.myMovieList) {
            if (Client.myMovieList.get(0).get_Revenue() == movie.get_Revenue()) {
                updateList.add(movie);
            }
        }
        TableController.matchedMovieList=updateList;
        stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("table.fxml"));
        scene = new Scene(fxmlLoader.load());
        stage.setTitle("In Table View");
        stage.setScene(scene);
    }

    public void onFindTotalProfit(ActionEvent actionEvent) throws IOException {
        for(Movie movie: Client.myMovieList)
        {
            sum+=movie.get_profit();
        }
        ProfitSceneController.total=sum;
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("profit-scene.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Here Goes Total Profit");
        stage.setScene(scene);


    }
    public void onInfoBackClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("menu-scene.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Manu Scene");
        stage.setScene(scene);
    }

    public  void onShowAllMoviesClick(ActionEvent actionEvent) throws IOException {

        TableController.matchedMovieList= Client.myMovieList;
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("table.fxml"));
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("In Table View");
        stage.setScene(scene);

    }
}
