package proxy;

public class GuildProxy implements GuildInterface {

    private final GuildInterface guild;

    public GuildProxy(GuildInterface guild) {
        this.guild = guild;
    }

    public JSON get(JSON query) {
        String hasQuery = "RedisHasQuery"; // Some parsing of query to check whether Redis (cache) already has information
        if (Redis.has(hasQuery)) {
            String getQuery = "RedisGetQuery"; // Get information from Redis (cache)
            return Redis.get(getQuery);
        }
        return this.guild.get(query); // Ask Postgres
    }
}
