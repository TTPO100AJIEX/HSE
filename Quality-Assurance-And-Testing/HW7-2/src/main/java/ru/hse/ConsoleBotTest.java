package ru.hse;

import ru.hse.bot.BotLogic;

import java.util.Scanner;

public class ConsoleBotTest {
    //local test
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in); //System.in is a standard input stream
        SimpleMemoryStorage sms = new SimpleMemoryStorage();
        BotLogic bl = new BotLogic();
        System.out.println(bl.reactOnMessage("",1, sms));
        String read = sc.nextLine();
        while(!read.equals("exit")){
            System.out.println(bl.reactOnMessage(read, 1, sms));
            read = sc.nextLine();
        }
    }
}
