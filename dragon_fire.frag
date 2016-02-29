varying vec3 ec_vnormal,ec_vposition,lightDir[3];

void lightGeometry(int light_index,float influence){
    vec3 P, N, L, V, H, R;
    vec4 ambient_color=gl_FrontMaterial.ambient;
    vec4 diffuse_color = gl_FrontMaterial.diffuse; 
    vec4 specular_color = gl_FrontMaterial.specular; 
    float shininess = gl_FrontMaterial.shininess;

    P = ec_vposition;
    N = normalize(ec_vnormal);
    L = normalize(lightDir[light_index]);
    V = normalize(-P);              // eye position is (0,0,0)!
    H = normalize(L+V);

    diffuse_color *= max(dot(N,L),0.0);
    //Blinn
    // specular_color *= pow(max(dot(H,N),0.0),shininess);
    //Phong
    R = reflect(-L,N);
    specular_color*=pow(max(dot(R,V),0.0),shininess/4.0);

    gl_FragColor += (ambient_color+diffuse_color + specular_color)
                     * influence;
}

void capFragColor() {
    gl_FragColor.x = min(gl_FragColor.x, 1.0);
    gl_FragColor.y = min(gl_FragColor.y, 1.0);
    gl_FragColor.z = min(gl_FragColor.z, 1.0);
}

void main (void)
{
  vec4 gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  
   lightGeometry(0,0.6);
   lightGeometry(1,0.3);
   lightGeometry(2,0.4);
   capFragColor();
}
