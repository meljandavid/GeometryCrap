// Remix of http://glslsandbox.com/e#68042.0
// remixed by https://www.dwitter.net/u/rodrigo.siqueira and meljandavid

#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 resolution;

void main() {
	vec2 uv = (gl_FragCoord.xy - resolution * 0.4) / max(resolution.x, resolution.y) * 2.25;
	
	float e = 0.0;
	for (float i=3.0; i<=15.0; i+=1.0) {
		e += 0.007/abs( (i/15.)+sin((time/2.0) + 0.15*i*(uv.x)*( cos(i/4.0 + time / 2.0 + uv.x*2.2) ) ) + 2.5*uv.y);
	}

	float blueish = gl_FragCoord.y / resolution.y - 0.2;
	gl_FragColor = vec4( vec3(0.0, e, min(1.0, e+blueish)), e/2.0);
}
	