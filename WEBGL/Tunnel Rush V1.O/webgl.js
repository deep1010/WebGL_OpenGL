
var max_level = 2, level = 1, pause = 0;
var speed_level = [0, 3, 5];
var quit = 0,move = 1;
var toggleGrayscale = 0,toggleColour = 0; //0 for Grayscale, 1 for Colourfull ,0 for keep it as it is, 1 for toggle
var frames = 0, colour = 0; //0 for original, 1 for shader
var shakey_frames = 120, level_frames = 1200;
var game_over = 0,score = 0;
var current_rotation = 0, amplitude = 0.007;
const pi=Math.PI;
var max_score=-1;

var count_shapes = 15;
var radius_object = 2;
var remove_offset = 5*radius_object;
var count_obstacles = 2;
var count_type_obstacles = 2;
var shapes_offset = 15*radius_object;

var camera_position = [0.0,-1.5, 0.0];
var camera_target = [0.0, -1.5, -1.0];
var camera_up = [0.0, 1.0, 0.0];

function initShaderProgram(gl, vsSource, fsSource) {
  const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);
  const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);

  // Create the shader program

  const shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  // If creating the shader program failed, alert

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
    return null;
  }

  return shaderProgram;
}

//
// creates a shader of the given type, uploads the source and
// compiles it.
//
function loadShader(gl, type, source) {
  const shader = gl.createShader(type);

  // Send the source to the shader object

  gl.shaderSource(shader, source);

  // Compile the shader program

  gl.compileShader(shader);

  // See if it compiled successfully

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
    return null;
  }

  return shader;
}
function create_octagon(radius,random_flag,black_flag){
    var face_color=[
    [1.0,  1.0,  1.0,  1.0],    
      [0.0,  0.0,  0.0,  1.0],  
      [1.0,  1.0,  1.0,  1.0],  
      [0.0,  0.0,  0.0,  1.0],  
      [1.0,  1.0,  1.0,  1.0],  
      [0.0,  0.0,  0.0,  1.0],  
      [1.0,  1.0,  1.0,  1.0],  
      [0.0,  0.0,  0.0,  1.0],  
      ];
      var index =[
      0,  1,  2,      0,  2,  3,    // right
      4,  5,  6,      4,  6,  7,    // right top
      8,  9,  10,     8,  10, 11,   // top
      12, 13, 14,     12, 14, 15,   // top left
      16, 17, 18,     16, 18, 19,   // left
      20, 21, 22,     20, 22, 23,   // bottom left
      24, 25, 26,     24, 26, 27,   // bottom
      28, 29, 30,     28, 30, 31,   // bottom right
    ];
    if(random_flag)
    {
      face_color = [
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Right face: random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Top Right face: random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Top face: random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Top Left Right face: random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Left face: random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Bottom Left face: random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Bottom face: random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    // Bottom Right face: random
      ];
    }
    else if(black_flag==1)
    {
      face_color = [
      [0.0,  0.0,  0.0,  1.0],   
      [1.0,  1.0,  1.0,  1.0],   
      [0.0,  0.0,  0.0,  1.0],   
      [1.0,  1.0,  1.0,  1.0],   
      [0.0,  0.0,  0.0,  1.0],   
      [1.0,  1.0,  1.0,  1.0],   
      [0.0,  0.0,  0.0,  1.0],   
      [1.0,  1.0,  1.0,  1.0],   
    ];

    }
    return {'position'  : [0, 0, 0],
    'radius' : 1/Math.cos(pi/8),
    'positions' : [
          // Right face
     radius, radius*Math.tan(pi/8), 0,
      radius, radius*Math.tan(pi/8), -2*radius,
      radius, radius*Math.tan(-pi/8), -2*radius,
      radius, radius*Math.tan(-pi/8), 0,
      // Top Right face
      radius*Math.tan(pi/8), radius, 0,
      radius*Math.tan(pi/8), radius, -2*radius,
      radius, radius*Math.tan(pi/8), -2*radius,
      radius, radius*Math.tan(pi/8), 0,
      // Top faces
      -radius*Math.tan(pi/8), radius, 0,
      -radius*Math.tan(pi/8), radius, -2*radius,
      radius*Math.tan(pi/8), radius, -2*radius,
      radius*Math.tan(pi/8), radius, 0,

      // Top Left face
      -radius, radius*Math.tan(pi/8), 0,
      -radius, radius*Math.tan(pi/8), -2*radius,
      -radius*Math.tan(pi/8), radius, -2*radius,
      -radius*Math.tan(pi/8), radius, 0,

      // Left fact
      -radius, radius*Math.tan(pi/8), 0,
      -radius, radius*Math.tan(pi/8), -2*radius,
      -radius, radius*Math.tan(-pi/8), -2*radius,
      -radius, radius*Math.tan(-pi/8), 0,

      // Bottom Left face
      -radius*Math.tan(pi/8), -radius, 0,
      -radius*Math.tan(pi/8), -radius, -2*radius,
      -radius, -radius*Math.tan(pi/8), -2*radius,
      -radius, -radius*Math.tan(pi/8), 0,

      // Bottom faces
      radius*Math.tan(pi/8), -radius, 0,
      radius*Math.tan(pi/8), -radius, -2*radius,
      -radius*Math.tan(pi/8), -radius, -2*radius,
      -radius*Math.tan(pi/8), -radius, 0,

      // Bottom Right face
      radius, -radius*Math.tan(pi/8), 0,
      radius, -radius*Math.tan(pi/8), -2*radius,
      radius*Math.tan(pi/8), -radius, -2*radius,
      radius*Math.tan(pi/8), -radius, 0,
      ],
      'textureCoordinates': [
       // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back
      ],
    'faceColors' : face_color,
    'vertexCount' : 48,
    'rotationZ' : 0,
    'rotationY' : 0,
    'rotationX' : 0,
    'rotation'  : 0.04,
    'speed'     : 7*radius,
    'indices' : index,
    'category'  : black_flag,
    'numComponentsColor' : 4,
    'numComponentsPosition' : 3,
  }
}

function create_cuboid(radius){
    var len = radius * Math.tan(pi/8)/3;
    // var radius = radius;
    var wid = radius * Math.tan(pi/8)/50;
    var temp_color =[
      [Math.random(),  Math.random(),  Math.random(),  1.0],    // Left face: random
      [Math.random(),  Math.random(),  Math.random(),  1.0],   // Bottom face: random
      [Math.random(),  Math.random(),  Math.random(),  1.0],    // Back face: random
      [Math.random(),  Math.random(),  Math.random(),  1.0],    // Front face: random
      [Math.random(),  Math.random(),  Math.random(),  1.0],// Top face: random
      [Math.random(),  Math.random(),  Math.random(),  1.0],// Right face: random
    ];
    return {'position'  : [0, 0, -20*radius],
    'positions' : [

      // Left face
      -len, radius, 0,
      -len, radius, -2*wid,
      -len, -radius, -2*wid,
      -len, -radius, 0,

      // Bottom faces
      -len, -radius, 0,
      len, -radius, 0,
      len, -radius, -2*wid,
      -len, -radius, -2*wid,

      // Back face
      -len, radius, -2*wid,
      len, radius, -2*wid,
      len, -radius, -2*wid,
      -len, -radius, -2*wid,

      // Front face
      -len, radius, 0,
      len, radius, 0,
      len, -radius, 0,
      -len, -radius, 0,


      // Top faces
      -len, radius, 0,
      len, radius, 0,
      len, radius, -2*wid,
      -len, radius, -2*wid,

      // Right face
      len, radius, 0,
      len, radius, -2*wid,
      len, -radius, -2*wid,
      len, -radius, 0,

    ],

    'faceColors' : temp_color,

    'indices' : [
      0,  1,  2,      0,  2,  3,    // right
      4,  5,  6,      4,  6,  7,    // left
      8,  9,  10,     8,  10, 11,   // top
      12, 13, 14,     12, 14, 15,   // bottom
      16, 17, 18,     16, 18, 19,   // front
      20, 21, 22,     20, 22, 23,   // back
    ],
    'textureCoordinates': [
       // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
      ],
    'rotation'  : (Math.floor(Math.random()*2)*2 - 1) * pi / 2.5 * Math.floor(Math.random() * (speed_level[level] + 1)),
    'vertexCount' : 36,
    'speed'     : 7*radius,
    'numComponentsPosition' : 3,
    'numComponentsColor' : 4,
    'rotationZ' : 0,
    'rotationY' : 0,
    'rotationX' : 0,
  }
}

function create_2triangles(radius){
    var len = radius * Math.tan(pi/8);
    var wid = radius * Math.tan(pi/8)/50;
    var turn=Math.floor(Math.random()*100);
    
    if(turn%2==0)
    {
      return{
      'position': [0, 0, -20*radius],
      'positions' : [
        // Top triangle

        // Left face
        0, 0, 0,
        0, 0, -2*wid,
        -len, radius, -2*wid,
        -len, radius, 0,


        // Back face
        -len, radius, -2*wid,
        len, radius, -2*wid,
        0, 0, -2*wid,
        len, radius, -2*wid,

        // Top faces
        -len, radius, 0,
        len, radius, 0,
        len, radius, -2*wid,
        -len, radius, -2*wid,

        // Front face
        -len, radius, 0,
        len, radius, 0,
        0, 0, 0,
        len, radius, 0,

        // Right face
        0, 0, 0,
        0, 0, -2*wid,
        len, radius, -2*wid,
        len, radius, 0,

        // Bottom triangle
        // Left face
        0, 0, 0,
        0, 0, -2*wid,
        -len, -radius, -2*wid,
        -len, -radius, 0,

        // Back face
        -len, -radius, -2*wid,
        len, -radius, -2*wid,
        0, 0, -2*wid,
        len, -radius, -2*wid,

        // Top faces
        -len, -radius, 0,
        len, -radius, 0,
        len, -radius, -2*wid,
        -len, -radius, -2*wid,

        // Front face
        -len, -radius, 0,
        len, -radius, 0,
        0, 0, 0,
        len, -radius, 0,

        // Right face
        0, 0, 0,
        0, 0, -2*wid,
        len, -radius, -2*wid,
        len, -radius, 0,
      ],

      'faceColors' : [
        // Top triangle
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        // Bottom triangle
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
      ],

      'indices' : [
        // Top triangle
        0,  1,  2,      0,  2,  3,    // right
        4,  5,  6,      4,  6,  7,    // left
        8,  9,  10,     8,  10, 11,   // top
        12, 13, 14,     12, 14, 15,   // front
        16, 17, 18,     16, 18, 19,   // back
        // Bottom triangle
        20, 21, 22,     20, 22, 23,   // right
        24, 25, 26,     24, 26, 27,    // left
        28, 29, 30,     28, 30, 31,   // top
        32, 33, 34,     32, 34, 35,   // front
        36, 37, 38,     36, 38, 39,   // back
      ],
    'textureCoordinates': [
       // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
        // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
      ],
      'vertexCount' : 60,
      'speed'     : 7*radius,
      'rotation'  : (Math.floor(Math.random()*2)*2 - 1) * pi / 2.5 * Math.floor(Math.random() * (speed_level[level] + 1)),
      'numComponentsColor' : 4,
      'numComponentsPosition' : 3,
      'rotationZ' : 0,
      'rotationY' : 0,
      'rotationX' : 0,}
    }
    else
    {
      return {
      'position'  : [0, 0, -20*radius],
      'positions' : [
        // Top triangle

        // Left face
        0, 0, 0,
        0, 0, -2*wid,
        -len, radius, -2*wid,
        -len, radius, 0,


        // Back face
        -len, radius, -2*wid,
        len, radius, -2*wid,
        0, 0, -2*wid,
        len, radius, -2*wid,

        // Top faces
        -len, radius, 0,
        len, radius, 0,
        len, radius, -2*wid,
        -len, radius, -2*wid,

        // Front face
        -len, radius, 0,
        len, radius, 0,
        0, 0, 0,
        len, radius, 0,

        // Right face
        0, 0, 0,
        0, 0, -2*wid,
        len, radius, -2*wid,
        len, radius, 0,

      ],

      'faceColors' : [
        // Top triangle
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
        [Math.random(),  Math.random(),  Math.random(),  1.0],    //  random
      ],

      'indices' : [
        // Top triangle
        0,  1,  2,      0,  2,  3,    // right
        4,  5,  6,      4,  6,  7,    // left
        8,  9,  10,     8,  10, 11,   // top
        12, 13, 14,     12, 14, 15,   // front
        16, 17, 18,     16, 18, 19,   // back
      ],
      'textureCoordinates': [
       // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    // Back // Front
    0.0,  0.0,
    1.0,  0.0,
    1.0,  1.0,
    0.0,  1.0,
    ],
      'rotation'  : (Math.floor(Math.random()*2)*2 - 1) * pi / 2.5 * Math.floor(Math.random() * (speed_level[level] + 1)),
      'speed'     : 7*radius,
      'vertexCount' : 30,
      'numComponentsColor' : 4,
      'numComponentsPosition' : 3,
      'rotationZ' : 0,
      'rotationY' : 0,
      'rotationX' : 0,
    }
  }
}

var count_shapes = 10;
var count_obstacles = 2;
var count_type_obstacles = 2;

main();

function main() {
  const canvas = document.querySelector('#glcanvas');
  const gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl');

  // If we don't have a GL context, give up now

  if (!gl) {
    alert('Unable to initialize WebGL. Your browser or machine may not support it.');
    return;
  }

  // Vertex shader program

  const vsSource = `
    attribute vec4 aVertexPosition;
    attribute vec4 aVertexColor;
    attribute vec2 aTextureCoord;

    uniform mat4 uModelViewMatrix;
    uniform mat4 uProjectionMatrix;

    varying lowp vec4 vColor;
    varying highp vec2 vTextureCoord;

    void main(void) {
      gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
      vColor = aVertexColor;
      vTextureCoord = aTextureCoord;
    }
  `;

  // Fragment shader program

  const fsSource = `
    varying highp vec2 vTextureCoord;

    uniform sampler2D uSampler;
    varying lowp vec4 vColor;

    void main(void) {
      gl_FragColor = vColor+texture2D(uSampler, vTextureCoord);
    }
  `;

  // Initialize a shader program; this is where all the lighting
  // for the vertices and so forth is established.
  const shaderProgram = initShaderProgram(gl, vsSource, fsSource);

  // Collect all the info needed to use the shader program.
  // Look up which attributes our shader program is using
  // for aVertexPosition, aVevrtexColor and also
  // look up uniform locations.
  const programInfo = {
    program: shaderProgram,
    attribLocations: {
      vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
      vertexColor: gl.getAttribLocation(shaderProgram, 'aVertexColor'),
      textureCoord: gl.getAttribLocation(shaderProgram, 'aTextureCoord'),
    },
    uniformLocations: {
      projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'),
      modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'),
      uSampler: gl.getUniformLocation(shaderProgram, 'uSampler'),
    },
  };

  // Here's where we call the routine that builds all the
  // objects we'll be drawing.

  shapes = [];
  buffer_shapes = [];
  obstacles = [];
  buffer_obstacles = [];

  for (var i = 0; i < count_shapes; i++){
      shapes.push(create_octagon(radius_object,toggleGrayscale,i%2));
      shapes[i].position[2] = -(2*i*radius_object + shapes_offset);
      buffer_shapes.push(initBuffers(gl, shapes[i]));
  }
  for (var i = 0; i < count_obstacles; i++){
      var type = Math.floor(Math.random()*100);

      if(type%2==0)
        obstacles.push(create_cuboid(radius_object));
      else
        obstacles.push(create_2triangles(radius_object));

      obstacles[i].rotationZ = i*pi/count_obstacles;
      obstacles[i].position[2] += -1*(shapes_offset + 10*(i-1)*radius_object);
      buffer_obstacles.push(initBuffers(gl, obstacles[i]));
  }
  const texture = loadTexture(gl, 'cubetexture.png');
  document.onkeydown = handleKeyDown;
  document.onkeyup = handleKeyUp;

  var then = 0;
  var theta = 0;
  // Draw the scene repeatedly
  function shakey_screen(now) {
    // requestAnimationFrame(render);
    frames+=1;
    shakey_frames-=1;
    const deltaTime = now*0.001 - then;
    now *= 0.001;  // convert to seconds
    then = now;
    const projectionMatrix = clearScene(gl);
    const x_coord=amplitude * Math.sin(2 * pi * frames / 4);
    var i=0;
    while(i < count_obstacles)
    {
        drawScene(gl, projectionMatrix, obstacles[i], programInfo, buffer_obstacles[i], deltaTime,texture);
        obstacles[i].position[0] = x_coord;
         i++;
    }
    i=0;
    while(i < count_shapes)
    {
        drawScene(gl, projectionMatrix, shapes[i], programInfo, buffer_shapes[i], deltaTime,texture);
        shapes[i].position[0] = x_coord;
        i++;
    }

    if(!quit && shakey_frames > 0){
        requestAnimationFrame(shakey_screen);
    }
  }

  // Draw the scene repeatedly
  function render(now) {
    // requestAnimationFrame(render);
    const deltaTime = now*0.001 - then;
    now = now*0.001;  // convert to seconds
    then = now;
    frames+=1;
    if(!(frames % level_frames))
      level= (level + 1 < max_level)?level+1:max_level;
    
    refresh_tunnel(gl, shapes, buffer_shapes);
    refresh_obstacles(gl, obstacles, buffer_obstacles);
    handleKeys(shapes, obstacles);
    
    const projectionMatrix = clearScene(gl);
    var i=0;
    while ( i < count_shapes)
    {
        drawScene(gl, projectionMatrix, shapes[i], programInfo, buffer_shapes[i], deltaTime,texture);
        shapes[i].position[2] +=  shapes[i++].speed * deltaTime;
    }
    i=0;
    while ( i < count_obstacles)
    {
        drawScene(gl, projectionMatrix, obstacles[i], programInfo, buffer_obstacles[i], deltaTime,texture);
        obstacles[i].position[2] += obstacles[i].speed * deltaTime;
        obstacles[i].rotationZ +=  obstacles[i++].rotation * deltaTime;
    }
    if(!(quit || detect_collision(shapes, obstacles)))
        requestAnimationFrame(render);

    else if(quit==0)
    {
        frames = 0;
        shakey_screen(gl, shapes, buffer_shapes, obstacles, buffer_obstacles);
    }
  }
  requestAnimationFrame(render);
}

function detect_collision(shapes, obstacles)
{
  var i=0;
    while(i < count_obstacles)
    {
      if(obstacles[i].position[2]  +0.5*radius_object > 0)
      {
        if(Math.abs(obstacles[i].rotationZ - Math.floor(obstacles[i].rotationZ / pi) * pi) <= pi / 8)
        {
          var score = Math.round(60 * frames / 60 * 100)/100;
          alert('Your final distance is:\t'+ score.toString()+'m\n');
          return true;
        }
        else
          return false;
      }
      i++;
    }
}

// Dictionary that keeps the track of the status of keys

var statusKeys = {};

function handleKeyDown(event)
{
  const key_press=event.keyCode;
  statusKeys[key_press] = true;
}

function handleKeyUp(event){
        
    if(event.keyCode == 71)
        toggleGrayscale = 1 - toggleGrayscale;
    else if(event.keyCode == 81)
        quit = 1;
    else
        statusKeys[event.keyCode] = false;
    
}


function handleKeys(shapes, obstacles){
    
  var flag=0;
    if(statusKeys[37])
      flag=1;

    else if(statusKeys[39])
      flag=-1;

      for(var i = 0; i < count_shapes; i++)
      {
        shapes[i].rotationZ += flag*shapes[i].rotation;
      }
      var angle=shapes[0].rotation;
      for(var i = 0; i < count_obstacles; i++)
      {
        obstacles[i].rotationZ += flag*angle;
      }

}


function initBuffers(gl, shape) {

  const positionBuffer = gl.createBuffer();
  const positions = shape.positions;
  const faceColors = shape.faceColors;
  const indices = shape.indices;
  const textureCoordinates = shape.textureCoordinates;
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

  var colors = [];
  var j=0,i=0;
  const face_length=faceColors.length;
  const num_colors= shape.numComponentsColor;
  var c;
  while(j<face_length)
  {
    c = faceColors[j];
    i=0;
    while(i < num_colors)
    {
      colors = colors.concat(c);
      i++;
    }
    j++;    
  }

  const colorBuffer = gl.createBuffer();
  const indexBuffer = gl.createBuffer();
  const textureCoordBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(indices), gl.STATIC_DRAW);
  gl.bindBuffer(gl.ARRAY_BUFFER, textureCoordBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoordinates),
                gl.STATIC_DRAW);

  return {
    color: colorBuffer,
    indices: indexBuffer,
    textureCoord: textureCoordBuffer,
    position: positionBuffer,
  };
}

function refresh_tunnel(gl, shapes, buffers)
{
  var l=shapes.length;
    if(shapes[0].position[2] > 2*remove_offset && l!=0)
    {
        buffers.shift();
        shapes.shift();
        count_shapes-=1;
        shapes.push(create_octagon(radius_object,toggleGrayscale,1-shapes[count_shapes-1].category));
        shapes[count_shapes ].position[2] = shapes[count_shapes - 1].position[2] - 2*radius_object;
        shapes[count_shapes ].rotationX   = shapes[count_shapes - 1].rotationX;
        shapes[count_shapes ].rotationY   = shapes[count_shapes - 1].rotationY;
        shapes[count_shapes ].rotationZ   = shapes[count_shapes - 1].rotationZ;
        buffers.push(initBuffers(gl, shapes[count_shapes]));
        count_shapes+=1;
    }
}

function refresh_obstacles(gl, obstacles, buffer_obstacles){
  var l=obstacles.length;
    if(l == 0){
        var type = Math.floor(Math.random()*100);

        if(type%2==0)
          obstacles.push(create_cuboid(radius_object));
               
        else
          obstacles.push(create_triangles(radius_object));
        
        count_obstacles++;
        obstacles[count_obstacles - 1].rotationZ = Math.random()*pi;
        buffer_obstacles.push(initBuffers(gl, obstacles[count_obstacles - 1]));        
    }

    else if(obstacles[0].position[2] > radius_object && l > 0 ){
        
        var type = Math.floor(Math.random()*100);
        count_obstacles-=1;
        
        buffer_obstacles.shift();
        obstacles.shift();
       
        if(type%2==0)
          obstacles.push(create_cuboid(radius_object));
               
        else
          obstacles.push(create_2triangles(radius_object));
        
        count_obstacles++;
        obstacles[count_obstacles - 1].rotationZ = Math.random()*pi;
        buffer_obstacles.push(initBuffers(gl, obstacles[count_obstacles - 1]));

    }
}

function clearScene(gl){
    gl.clearDepth(1.0);                 // Clear everything
    gl.clearColor(0, 0, 0, 1.0);  // Clear to black, fully opaque
    gl.enable(gl.DEPTH_TEST);           // Enable depth testing
    gl.depthFunc(gl.LEQUAL);            // Near things obscure far things


    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
    const fieldOfView = 45 * pi;   // in radians
    const projectionMatrix = mat4.create();
mat4.translate(projectionMatrix,projectionMatrix,camera_position);
    mat4.lookAt(projectionMatrix,camera_position,camera_target,camera_up);
    mat4.perspective(projectionMatrix,
                     fieldOfView/180,
                     aspect,
                     0.1,
                     100.0);
    return projectionMatrix;
}

function drawScene(gl, projectionMatrix, shape, programInfo, buffers, deltaTime,texture) {
  // Set the drawing position to the "identity" point, which is
  // the center of the scene.
  const modelViewMatrix = mat4.create();

  // Now move the drawing position a bit to where we want to
  // start drawing the square.
  mat4.lookAt(modelViewMatrix, camera_position, camera_target, camera_up);
  const X_angle=shape.rotationX;
  const Y_angle=shape.rotationY;
  const Z_angle=shape.rotationZ;
  mat4.translate(modelViewMatrix,     
                 modelViewMatrix,     
                 shape.position);  // amount to translate
  
  mat4.rotate(modelViewMatrix,  // destination matrix
              modelViewMatrix,  // matrix to rotate
              Z_angle,// amount to rotate in radians
              [0, 0, 1]);       // axis to rotate around (Z)
  
  mat4.rotate(modelViewMatrix,  // destination matrix
              modelViewMatrix,  // matrix to rotate
              Y_angle,// amount to rotate in radians
              [0, 1, 0]);       // axis to rotate around (Y)
  
  mat4.rotate(modelViewMatrix,  // destination matrix
              modelViewMatrix,  // matrix to rotate
              X_angle,     // amount to rotate in radians
              [1, 0, 0]);       // axis to rotate around (X)

  // Tell WebGL how to pull out the positions from the position
  // buffer into the vertexPosition attribute
  {
    var numComponents = shape.numComponentsPosition, type = gl.FLOAT, normalize = false;
    gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
    gl.vertexAttribPointer(
        programInfo.attribLocations.vertexPosition,
        numComponents,
        type,
        normalize,
        0,
        0);
    gl.enableVertexAttribArray(
        programInfo.attribLocations.vertexPosition);
  }

  {
    gl.bindBuffer(gl.ARRAY_BUFFER, buffers.color);
    gl.vertexAttribPointer(
        programInfo.attribLocations.vertexColor,
        shape.numComponentsColor,
        gl.FLOAT,
        false,
        0,
        0);
    gl.enableVertexAttribArray(
        programInfo.attribLocations.vertexColor);
  }
  // tell webgl how to pull out the texture coordinates from buffer
  {
      const num = 2; // every coordinate composed of 2 values
      const type = gl.FLOAT; // the data in the buffer is 32 bit float
      const normalize = false; // don't normalize
      const stride = 0; // how many bytes to get from one set to the next
      const offset = 0; // how many bytes inside the buffer to start from
      gl.bindBuffer(gl.ARRAY_BUFFER, buffers.textureCoord);
      gl.vertexAttribPointer(programInfo.attribLocations.textureCoord, num, type, normalize, stride, offset);
      gl.enableVertexAttribArray(programInfo.attribLocations.textureCoord);
  }

  // Tell WebGL which indices to use to index the vertices
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, buffers.indices);

  // Tell WebGL to use our program when drawing

  gl.useProgram(programInfo.program);

  // Set the shader uniforms
  var flag;
  gl.uniformMatrix4fv(
      programInfo.uniformLocations.projectionMatrix,
      flag = false,
      projectionMatrix);
  gl.uniformMatrix4fv(
      programInfo.uniformLocations.modelViewMatrix,
      flag,
      modelViewMatrix);

   // Tell WebGL we want to affect texture unit 0
  gl.activeTexture(gl.TEXTURE0);

  // Bind the texture to texture unit 0
  gl.bindTexture(gl.TEXTURE_2D, texture);

  // Tell the shader we bound the texture to texture unit 0
  gl.uniform1i(programInfo.uniformLocations.uSampler, 0);

  {
    var vertexCount = shape.vertexCount,type = gl.UNSIGNED_SHORT,offset = 0;
    gl.drawElements(gl.TRIANGLES, vertexCount, type, 0);
  }

}
function loadTexture(gl, url) {
  const texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture);

  // Because images have to be download over the internet
  // they might take a moment until they are ready.
  // Until then put a single pixel in the texture so we can
  // use it immediately. When the image has finished downloading
  // we'll update the texture with the contents of the image.
  const level = 0;
  const internalFormat = gl.RGBA;
  const width = 1;
  const height = 1;
  const border = 0;
  const srcFormat = gl.RGBA;
  const srcType = gl.UNSIGNED_BYTE;
  const pixel = new Uint8Array([0, 0, 255, 255]);  // opaque blue
  gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
                width, height, border, srcFormat, srcType,
                pixel);

  const image = new Image();
  image.onload = function() {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
                  srcFormat, srcType, image);

    // WebGL1 has different requirements for power of 2 images
    // vs non power of 2 images so check if the image is a
    // power of 2 in both dimensions.
    if (isPowerOf2(image.width) && isPowerOf2(image.height)) {
       // Yes, it's a power of 2. Generate mips.
       gl.generateMipmap(gl.TEXTURE_2D);
    } else {
       // No, it's not a power of 2. Turn of mips and set
       // wrapping to clamp to edge
       gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
       gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
       gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    }
  };
  image.src = url;

  return texture;
}

function isPowerOf2(value) {
  return (value & (value - 1)) == 0;
}