package reversi.utils.Renderer;

import reversi.utils.Renderer.Items.Line;
import reversi.utils.Renderer.Items.Text;

import java.util.ArrayList;

public interface Renderer {
    void clear();

    void render(final Item context);

    void render(final Context context);

    enum Colors {
        // Reset, None
        RESET, NONE,
        // Regular Colors
        BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE,
        // Bold
        BLACK_BOLD, RED_BOLD, GREEN_BOLD, YELLOW_BOLD, BLUE_BOLD, PURPLE_BOLD, CYAN_BOLD, WHITE_BOLD,
        // Underline
        BLACK_UNDERLINED, RED_UNDERLINED, GREEN_UNDERLINED, YELLOW_UNDERLINED, BLUE_UNDERLINED, PURPLE_UNDERLINED, CYAN_UNDERLINED, WHITE_UNDERLINED,
        // Background
        BLACK_BACKGROUND, RED_BACKGROUND, GREEN_BACKGROUND, YELLOW_BACKGROUND, BLUE_BACKGROUND, PURPLE_BACKGROUND, CYAN_BACKGROUND, WHITE_BACKGROUND,
        // High Intensity
        BLACK_BRIGHT, RED_BRIGHT, GREEN_BRIGHT, YELLOW_BRIGHT, BLUE_BRIGHT, PURPLE_BRIGHT, CYAN_BRIGHT, WHITE_BRIGHT,
        // Bold High Intensity
        BLACK_BOLD_BRIGHT, RED_BOLD_BRIGHT, GREEN_BOLD_BRIGHT, YELLOW_BOLD_BRIGHT, BLUE_BOLD_BRIGHT, PURPLE_BOLD_BRIGHT, CYAN_BOLD_BRIGHT, WHITE_BOLD_BRIGHT,
        // High Intensity backgrounds
        BLACK_BACKGROUND_BRIGHT, RED_BACKGROUND_BRIGHT, GREEN_BACKGROUND_BRIGHT, YELLOW_BACKGROUND_BRIGHT, BLUE_BACKGROUND_BRIGHT, PURPLE_BACKGROUND_BRIGHT, CYAN_BACKGROUND_BRIGHT, WHITE_BACKGROUND_BRIGHT
    }

    interface Item {
        // TODO: GetVertexArray(), GetTransform(), GetShader(), etc.

        default String getData() {
            return "";
        }

        default Colors getColor() {
            return Colors.NONE;
        }

        default Context getSubcontext() {
            return new Context();
        }
    }

    final class Context {
        private final ArrayList<Item> items = new ArrayList<Item>();


        public Context add(final Item item) {
            this.items.add(item);
            return this;
        }

        public Context addText() {
            return this.add(new Text());
        }

        public Context addText(final String text) {
            return this.add(new Text(text));
        }

        public Context addText(final String text, final Colors color) {
            return this.add(new Text(text, color));
        }

        public Context addLine() {
            return this.add(new Line());
        }

        public Context addLine(final String text) {
            return this.add(new Line(text));
        }

        public Context addLine(final String text, final Colors color) {
            return this.add(new Line(text, color));
        }


        ArrayList<Item> getItems() {
            return this.items;
        }

        public Context remove(int index) {
            index %= this.items.size();
            if (index < 0) index += this.items.size();
            this.items.remove(index);
            return this;
        }

        public Context clear() {
            this.items.clear();
            return this;
        }
    }
}
