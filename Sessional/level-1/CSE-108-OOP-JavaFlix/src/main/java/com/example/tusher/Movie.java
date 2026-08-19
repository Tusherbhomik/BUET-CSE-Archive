package com.example.tusher;

import java.io.Serializable;

public class Movie implements Serializable {
    private String name;
    private String releaseYear;
    private String genre1;
    private String genre2;
    private String genre3;
    private String runningTime;
    private String productionCompany;
    private String budget;
    private String revenue;

    public void setName(String name) {
        this.name = name;
    }

    public void setReleaseYear(String releaseYear) {
        this.releaseYear = releaseYear;
    }

    public void setGenre1(String genre1) {
        this.genre1 = genre1;
    }

    public void setGenre2(String genre2) {
        this.genre2 = genre2;
    }

    public void setGenre3(String genre3) {
        this.genre3 = genre3;
    }

    public void setRunningTime(String runningTime) {
        this.runningTime = runningTime;
    }

    public void setProductionCompany(String productionCompany) {
        this.productionCompany = productionCompany;
    }

    public void setBudget(String budget) {
        this.budget = budget;
    }

    public void setRevenue(String revenue) {
        this.revenue = revenue;
    }
    public Movie(String Name, String releaseYear, String genre1, String genre2, String genre3, String runningTime,
                 String productionCompany, String budget, String revenue){
        this.name =(Name);
        this.releaseYear =releaseYear;
        this.genre1 =genre1;
        this.genre2 =genre2;
        this.genre3 =genre3;
        this.runningTime =runningTime;
        this.productionCompany =productionCompany;
        this.budget =budget;
        this.revenue =revenue;
    }
    public Movie()
    {
        //default cons
    }
    public String getName() {
        return name;
    }

    public String getReleaseYear() {
        return releaseYear;
    }
    public int get_year() {
        return Integer.parseInt(releaseYear);
    }
    public String getGenre1() {
        return genre1;
    }

    public String getGenre2() {
        return genre2;
    }

    public String getGenre3() {
        return genre3;
    }

    public String getRunningTime() {
        return runningTime;
    }

    public String getProductionCompany() {
        return productionCompany;
    }

    public String getBudget() {
        return budget;
    }

    public String getRevenue() {
        return revenue;
    }

    public int get_Revenue() {
        return Integer.parseInt(revenue);
    }
    public int get_runtime()
    {
        return Integer.parseInt(runningTime);
    }
    public int get_profit() {
        return ( Integer.parseInt(revenue)- Integer.parseInt(budget));
    }

}

