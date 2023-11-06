package proxy;

public class Guild implements GuildInterface {

    private final String id;

    public Guild(String id) {
        this.id = id;
    }

    public JSON get(JSON query) {
        String sql = "SQL"; // Some parsing of query to SQL
        return PostgreSQL.query(sql);
    }
}
