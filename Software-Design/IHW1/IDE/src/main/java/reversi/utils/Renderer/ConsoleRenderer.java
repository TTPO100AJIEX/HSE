package reversi.utils.Renderer;

public final class ConsoleRenderer implements Renderer {
    private static String parseColor(Colors color) {
        if (color == Colors.NONE) return "";
        if (color == Colors.RESET) return "\033[0m";
        String name = color.name(), colorname = name, typename = "";
        if (name.contains("_")) {
            colorname = name.substring(0, name.indexOf("_"));
            typename = name.substring(name.indexOf("_") + 1);
        }

        String answer = "\033[";
        switch (typename) {
            case "BOLD": {
                answer += "1;3";
                break;
            }
            case "UNDERLINED": {
                answer += "4;3";
                break;
            }
            case "BACKGROUND": {
                answer += "4";
                break;
            }
            case "BRIGHT": {
                answer += "0;9";
                break;
            }
            case "BOLD_BRIGHT": {
                answer += "1;9";
                break;
            }
            case "BACKGROUND_BRIGHT": {
                answer += "0;10";
                break;
            }
            default: {
                answer += "0;3";
                break;
            }
        }
        switch (colorname) {
            case "BLACK": {
                answer += "0m";
                break;
            }
            case "RED": {
                answer += "1m";
                break;
            }
            case "GREEN": {
                answer += "2m";
                break;
            }
            case "YELLOW": {
                answer += "3m";
                break;
            }
            case "BLUE": {
                answer += "4m";
                break;
            }
            case "PURPLE": {
                answer += "5m";
                break;
            }
            case "CYAN": {
                answer += "6m";
                break;
            }
            default: {
                answer += "7m";
                break;
            }
        }

        return answer;
    }

    @Override
    public void clear() {
        try {
            if (System.getProperty("os.name").contains("Windows"))
                new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
            else new ProcessBuilder("bash", "-c", "clear").inheritIO().start().waitFor();
        } catch (Exception err) {
        }
    }

    @Override
    public void render(final Item item) {
        if (item == null) return;
        System.out.print(ConsoleRenderer.parseColor(item.getColor()) + item.getData() + ConsoleRenderer.parseColor(Colors.RESET));
        this.render(item.getSubcontext());
    }

    @Override
    public void render(final Context context) {
        if (context == null) return;
        for (Item item : context.getItems()) this.render(item);
    }
}
