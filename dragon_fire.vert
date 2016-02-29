varying vec3 ec_vnormal, ec_vposition,lightDir[3];

vec4 quat_from_axis_angle(vec3 axis, float angle)
{ 
  vec4 qr;
  float half_angle = (angle * 0.5) * 3.14159 / 180.0;
  qr.x = axis.x * sin(half_angle);
  qr.y = axis.y * sin(half_angle);
  qr.z = axis.z * sin(half_angle);
  qr.w = cos(half_angle);
  return qr;
}
 
vec3 rotate_vertex_position(vec3 position, vec3 axis, float angle)
{ 
  vec4 q = quat_from_axis_angle(axis, angle);
  vec3 v = position.xyz;
  return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main()
{		
  ec_vnormal = gl_NormalMatrix * gl_Normal;
  ec_vposition = gl_ModelViewMatrix * gl_Vertex;
  vec3 v=vec3(3.0,3.0,3.0);
  vec3 center=vec3(v.x/2,v.y/2,v.z/2);
  vec3 origin=center-v;
  vec3 up=vec3(v.x/2,3,v.z/2);
  vec3 cross=cross(origin,up);
  //key
  vec3 key=rotate_vertex_position(origin,cross,35.0);
  key=rotate_vertex_position(key,up,15.0);
  //fill
   vec3 fill=rotate_vertex_position(origin,cross,20.0);
  fill=rotate_vertex_position(fill,up,-45.0);
  //rim
   vec3 rim=rotate_vertex_position(origin,cross,25.0);
  rim=rotate_vertex_position(rim,up,180.0);


  gl_LightSource[0].position.xyz=key.xyz;
  gl_LightSource[1].position.xyz=fill.xyz;
  gl_LightSource[2].position.xyz=rim.xyz;

  int i;
  for (i=0; i<3; ++i)
    lightDir[i] = 
      vec3(gl_LightSource[i].position.xyz - ec_vposition);
  gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
}
