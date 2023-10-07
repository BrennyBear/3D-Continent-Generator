#version 330 core
out vec4 FragColor;

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform bool colorMap;

// 2D Random
vec2 random2(vec2 st, float l_time){
st *= l_time;
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.54531235);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise(vec2 st, float l_time) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0), l_time ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0), l_time ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0), l_time ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0), l_time ), f - vec2(1.0,1.0) ), u.x), u.y);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    vec3 color = vec3(0.0);

    float l_time = u_time;


        vec2 pos = vec2(st * 2);

        color += vec3((noise(pos, l_time))*0.25 + 0.25);

        //pos = vec2(st * 2);

        //color += vec3((noise(pos, l_time)));

        pos = vec2(st * 3);

        color += vec3((noise(pos, l_time)));

        //pos = vec2(st * 4);

        //color += vec3((noise(pos, l_time)));

        vec2 screenCoord = gl_FragCoord.xy/u_resolution.xy;
        
        //screenCoord -= 0.5;

        //screenCoord *= 2;

        //screenCoord.x = 0.9 - pow(screenCoord.x,2);

        //screenCoord.y = 0.9 - pow(screenCoord.y,2);

        //color *= screenCoord.x * screenCoord.y;


        for (int i = 0; i <= 2; i++)
    {
        pos = vec2(st * pow(2, float(i)));

        color += vec3((noise(pos, l_time)) / pow(1.55, float(i)));

        l_time += 3;
    }

    screenCoord = gl_FragCoord.xy/u_resolution.xy;

    screenCoord -= 0.5;

    screenCoord *= 2;

    screenCoord.x = 0.9 - pow(screenCoord.x,2);

    screenCoord.y = 0.9 - pow(screenCoord.y,2);

    color *= screenCoord.x * screenCoord.y;

    for (int i = 2; i <= 12; i++)
    {
        pos = vec2(st * pow(2, float(i+1)));

        color += vec3((noise(pos, l_time)) / pow(1.85, float(i)));
    }

    screenCoord = gl_FragCoord.xy/u_resolution.xy;

    screenCoord -= 0.5;

    screenCoord *= 2;

    screenCoord.x = 1 - pow(screenCoord.x,2);

    screenCoord.y = 1 - pow(screenCoord.y,2);

    color *= screenCoord.x * screenCoord.y;

    if(colorMap == false)
    {
        if(color.x < 0.0625)
        //if(color.x < 0.0f)
        {
            color.x = 0.0;
    
        }
    }
    else if(colorMap == true)
    {
    
        //if(color.x <  0)
        //{
        //    color.x = 0;
        //    color.y = 0;
        //    color.z = 0;
        //}
        if(color.x < 0.0625)
        //if( color.x < 0.0f)
        {
            color.x = 0;
            color.y = 0;
            color.z =0.5 + color.z *0.5;
        }
        else if(color.x < 0.125)
        {
            color.x = sqrt(color.x);
            color.y = sqrt(color.y);
            color.z = 0;

        }
        else if(color.x < 0.25)
        {
            color.x = 0;
            color.y = sqrt(color.y);
            color.z = 0;
        }
        else if(color.x < 0.45)
        {
            color.x = sqrt(color.x)/1.2;
            color.y = sqrt(color.y)/1.5;
        }
        else
        {
            color.x = 0.9;
            color.y = 0.9;
            color.z = 0.9;
        }
    }


    gl_FragColor = vec4(color,1.0);
}
