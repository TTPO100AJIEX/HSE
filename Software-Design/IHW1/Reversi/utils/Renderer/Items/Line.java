package reversi.utils.Renderer.Items;

import reversi.utils.Renderer.Renderer.Item;
import reversi.utils.Renderer.Renderer.Colors;

public class Line implements Item
{
    private String text = "";
    private Colors color = Colors.NONE;
    public Line() { }
    public Line(final String text) { this.text = text; }
    public Line(final String text, final Colors color) { this.text = text; this.color = color; }

    @Override public String getData() { return (this.text + "\n"); }
    @Override public Colors getColor() { return this.color; }
}