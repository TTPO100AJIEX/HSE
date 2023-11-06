package reversi.utils.Renderer.Items;

import reversi.utils.Renderer.Renderer.Item;
import reversi.utils.Renderer.Renderer.Colors;

public class Text implements Item
{
    private String text = "";
    private Colors color = Colors.NONE;
    public Text() { }
    public Text(final String text) { this.text = text; }
    public Text(final String text, final Colors color) { this.text = text; this.color = color; }

    @Override public String getData() { return this.text; }
    @Override public Colors getColor() { return this.color; }
}