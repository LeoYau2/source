class AutoSolving{
    Game game_instance;
    public AutoSolving(Game new_game){
        newGame(new_game);
        ....
    }
    public int nextStep(){ //每次執行時完成圖中的一輪操作
        ....
        return 先前作業所述的遊戲狀態數值;
    }
    public int Solving(){  //無限執行判斷與選擇直到遊戲結束
        while(nextStep() == End_Game); //End_Game有兩種可能 勝利或失敗
        return 勝利或失敗的遊戲狀態數值;
    }
    public void newGame(Game new_game){
        game_instance = new_game;
    }
}

public int nextStep(Game new_game){ //每次執行時完成圖中的一輪操作
        ....
        return 先前作業所述的遊戲狀態數值;
    }

public int Solving(Game new_game){  //無限執行判斷與選擇直到遊戲結束
    while(nextStep() == End_Game); //End_Game有兩種可能 勝利或失敗
    return 勝利或失敗的遊戲狀態數值;
}