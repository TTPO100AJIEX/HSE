package proxy;

public class ProxyTest {

    public static void main(String[] args) {
        GuildInterface guild = new Guild("563733646440267827");
        GuildInterface cache = new GuildProxy(guild);
        JSON data = cache.get(new JSON());
    }
}