in highp vec4 f_Color;
out highp vec4 o_Color;
uniform highp vec4 u_Color;

void main()
{
    o_Color = f_Color * u_Color;
}
