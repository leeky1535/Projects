
var gl;
var canvas;
var shaderProgram;
var vertexPositionBuffer;

var keysDown = [];

var lastTime = -1;
var airSpeed = 1;
// degrees per second
var turnSpeed = 25;
// keep track of the last time movement was processed, in microseconds


// Create a place to store terrain geometry
var tVertexPositionBuffer;

//Create a place to store normals for shading
var tVertexNormalBuffer;

// Create a place to store the terrain triangles
var tIndexTriBuffer;

// View parameters
var eyePt = vec3.fromValues(0.0,0.0,6.0);
var up = vec3.fromValues(0.0,0.0,1.0);
var viewPt = vec3.fromValues(0.0,4.0,5.0);

// Create the normal
var nMatrix = mat3.create();

// Create ModelView matrix
var mvMatrix = mat4.create();

//Create Projection matrix
var pMatrix = mat4.create();

var mvMatrixStack = [];
var dsQueue = [];


function diamondstep(vTerrain, p1, p2, p3, p4, randomvalue, gridN, mid) {               //diamondstep a square

    var average = (vTerrain[(p1[1]*(gridN+1)+p1[0])*3+2] + vTerrain[(p2[1]*(gridN+1)+p2[0])*3+2] + vTerrain[(p3[1]*(gridN+1)+p3[0])*3+2] + vTerrain[(p4[1]*(gridN+1)+p4[0])*3+2]) / 4;
    vTerrain[(mid[1]*(gridN+1)+mid[0])*3+2] = average + randomvalue;
}

function squarestep(vTerrain, p1, p2, p3, p4, randomvalue, gridN, mid) {               //squarestep a side of a square

    var average;
    if(p4[0] < 0 || p4[0] > gridN || p4[1] < 0 || p4[1] > gridN){
        average = (vTerrain[(p1[1]*(gridN+1)+p1[0])*3+2] + vTerrain[(p2[1]*(gridN+1)+p2[0])*3+2] + vTerrain[(p3[1]*(gridN+1)+p3[0])*3+2]) / 3;
    }
    else{
        average = (vTerrain[(p1[1]*(gridN+1)+p1[0])*3+2] + vTerrain[(p2[1]*(gridN+1)+p2[0])*3+2] + vTerrain[(p3[1]*(gridN+1)+p3[0])*3+2] + vTerrain[(p4[1]*(gridN+1)+p4[0])*3+2]) / 4;
    }
    vTerrain[(mid[1]*(gridN+1)+mid[0])*3+2] = average + randomvalue;    
}


function diamondsquare(vTerrain, p1, p2, p3, p4, width, randomvalue, gridN) {     //diamondsquare a square
    randomvalue = randomvalue - Math.random()*0.03;
    var mid = [(p1[0]+p2[0]) / 2, (p1[1]+p3[1]) / 2];
    diamondstep(vTerrain, p1, p2, p3, p4, randomvalue, gridN, mid);
    
    var mid1 = [mid[0], p1[1]];
    var mid2 = [p1[0], mid[1]];
    var mid3 = [p2[0], mid[1]];
    var mid4 = [mid[0], p3[1]];
    var pout1 = [];
    var pout2 = [];
    var pout3 = [];
    var pout4 = [];
    
    randomvalue = randomvalue - Math.random()*0.03;                                      //squarestep four sides
    pout4 = [mid[0], mid[1]+width];
    squarestep(vTerrain, p3, mid, p4, pout4, randomvalue, gridN, mid4);          
    
    pout3 = [mid[0]+width, mid[1]];
    squarestep(vTerrain, p2, mid, p4, pout3, randomvalue, gridN, mid3);
    
    pout2 = [mid[0]-width, mid[1]];
    squarestep(vTerrain, p1, mid, p3, pout2, randomvalue, gridN, mid2);
    
    pout1 = [mid[0], mid[1]-width];
    squarestep(vTerrain, p1, mid, p2, pout1, randomvalue, gridN, mid1);
}

function diamondsquareloop(vTerrain, dsQueue, randomvalue, gridN){           //diamondsquare algorithm using queue
     while(dsQueue.length >0){
        var copy = [];
        copy = dsQueue.shift();
        diamondsquare(vTerrain, copy[0], copy[1], copy[2], copy[3], copy[1][0]-copy[0][0], randomvalue, gridN);
        if(copy[1][0]-copy[0][0] >= 2){
            var mid = [(copy[0][0]+copy[1][0]) / 2, (copy[0][1]+copy[2][1]) / 2];
            var mid1 = [mid[0], copy[0][1]];
            var mid2 = [copy[0][0], mid[1]];
            var mid3 = [copy[1][0], mid[1]];
            var mid4 = [mid[0], copy[2][1]];
        
            var arr1 = [copy[0], mid1, mid3, mid];
            var arr2 = [mid1, copy[1], mid, mid3];
            var arr3 = [mid2, mid, copy[2], mid4];
            var arr4 = [mid, mid3, mid4, copy[3]];
            dsQueue.push(arr4);
            dsQueue.push(arr3);
            dsQueue.push(arr2);
            dsQueue.push(arr1);
        }
    }
}

function handleKeyDown(event) {
	if(event.keyCode == 76)
		useLighting = !useLighting;
	keysDown[event.keyCode] = true;
}

function handleKeyUp(event) {
	keysDown[event.keyCode] = false;
}

//-------------------------------------------------------------------------
function setupTerrainBuffers() {
    
    var vTerrain=[];
    var fTerrain=[];
    var nTerrain=[];
    var eTerrain=[];
    var gridN=128;              //initialize four courners to do diamondsquare
    var p1 = [0, 0];
    var p2 = [gridN, 0];
    var p3 = [0, gridN];
    var p4 = [gridN, gridN];
    

    
    var numT = terrainFromIteration(gridN, -16,16,-16,16, vTerrain, fTerrain, nTerrain);
    vTerrain[2] = 0;                       //height values of the four corners
    vTerrain[gridN*3+2] = 2;
    vTerrain[gridN*(gridN+1)*3+2] = 1;
    vTerrain[(gridN*(gridN+1)+20)*3+2] = 3;
    
    var copy = [p1, p2, p3, p4];           //do diamondsquare algorithm using queue
    dsQueue.push(copy);
    diamondsquareloop(vTerrain, dsQueue, 0.5, gridN);
    
    tVertexPositionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, tVertexPositionBuffer);      
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vTerrain), gl.STATIC_DRAW);
    tVertexPositionBuffer.itemSize = 3;
    tVertexPositionBuffer.numItems = (gridN+1)*(gridN+1);
    
    // Specify normals to be able to do lighting calculations
    tVertexNormalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, tVertexNormalBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(nTerrain),
                  gl.STATIC_DRAW);
    tVertexNormalBuffer.itemSize = 3;
    tVertexNormalBuffer.numItems = (gridN+1)*(gridN+1);
    
    // Specify faces of the terrain 
    tIndexTriBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, tIndexTriBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(fTerrain),
                  gl.STATIC_DRAW);
    tIndexTriBuffer.itemSize = 1;
    tIndexTriBuffer.numItems = numT*3;
    
    
    
     
}


//-------------------------------------------------------------------------
function drawTerrain(){
 gl.polygonOffset(0,0);
 gl.bindBuffer(gl.ARRAY_BUFFER, tVertexPositionBuffer);
 gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, tVertexPositionBuffer.itemSize, 
                         gl.FLOAT, false, 0, 0);

 // Bind normal buffer
 gl.bindBuffer(gl.ARRAY_BUFFER, tVertexNormalBuffer);
 gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute,
                           tVertexNormalBuffer.itemSize,
                           gl.FLOAT, false, 0, 0);   
    
 //Draw 
 gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, tIndexTriBuffer);
 gl.drawElements(gl.TRIANGLES, tIndexTriBuffer.numItems, gl.UNSIGNED_SHORT,0);      
}


//-------------------------------------------------------------------------
function uploadModelViewMatrixToShader() {
  gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
}

//-------------------------------------------------------------------------
function uploadProjectionMatrixToShader() {
  gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, 
                      false, pMatrix);
}

//-------------------------------------------------------------------------
function uploadNormalMatrixToShader() {
  mat3.fromMat4(nMatrix,mvMatrix);
  mat3.transpose(nMatrix,nMatrix);
  mat3.invert(nMatrix,nMatrix);
  gl.uniformMatrix3fv(shaderProgram.nMatrixUniform, false, nMatrix);
}

//----------------------------------------------------------------------------------
function mvPushMatrix() {
    var copy = mat4.clone(mvMatrix);
    mvMatrixStack.push(copy);
}


//----------------------------------------------------------------------------------
function mvPopMatrix() {
    if (mvMatrixStack.length == 0) {
      throw "Invalid popMatrix!";
    }
    mvMatrix = mvMatrixStack.pop();
}

//----------------------------------------------------------------------------------
function setMatrixUniforms() {
    uploadModelViewMatrixToShader();
    uploadNormalMatrixToShader();
    uploadProjectionMatrixToShader();
}

//----------------------------------------------------------------------------------
function degToRad(degrees) {
        return degrees * Math.PI / 180;
}

//----------------------------------------------------------------------------------
function createGLContext(canvas) {
  var names = ["webgl", "experimental-webgl"];
  var context = null;
  for (var i=0; i < names.length; i++) {
    try {
      context = canvas.getContext(names[i]);
    } catch(e) {}
    if (context) {
      break;
    }
  }
  if (context) {
    context.viewportWidth = canvas.width;
    context.viewportHeight = canvas.height;
  } else {
    alert("Failed to create WebGL context!");
  }
  return context;
}

//----------------------------------------------------------------------------------
function loadShaderFromDOM(id) {
  var shaderScript = document.getElementById(id);
  
  // If we don't find an element with the specified id
  // we do an early exit 
  if (!shaderScript) {
    return null;
  }
  
  // Loop through the children for the found DOM element and
  // build up the shader source code as a string
  var shaderSource = "";
  var currentChild = shaderScript.firstChild;
  while (currentChild) {
    if (currentChild.nodeType == 3) { // 3 corresponds to TEXT_NODE
      shaderSource += currentChild.textContent;
    }
    currentChild = currentChild.nextSibling;
  }
 
  var shader;
  if (shaderScript.type == "x-shader/x-fragment") {
    shader = gl.createShader(gl.FRAGMENT_SHADER);
  } else if (shaderScript.type == "x-shader/x-vertex") {
    shader = gl.createShader(gl.VERTEX_SHADER);
  } else {
    return null;
  }
 
  gl.shaderSource(shader, shaderSource);
  gl.compileShader(shader);
 
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(shader));
    return null;
  } 
  return shader;
}

//----------------------------------------------------------------------------------
function setupShaders() {
  vertexShader = loadShaderFromDOM("shader-vs");
  fragmentShader = loadShaderFromDOM("shader-fs");
  
  shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert("Failed to setup shaders");
  }

  gl.useProgram(shaderProgram);

  shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
  gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

  shaderProgram.vertexNormalAttribute = gl.getAttribLocation(shaderProgram, "aVertexNormal");
  gl.enableVertexAttribArray(shaderProgram.vertexNormalAttribute);

  shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
  shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
  shaderProgram.nMatrixUniform = gl.getUniformLocation(shaderProgram, "uNMatrix");
  shaderProgram.uniformLightPositionLoc = gl.getUniformLocation(shaderProgram, "uLightPosition");    
  shaderProgram.uniformAmbientLightColorLoc = gl.getUniformLocation(shaderProgram, "uAmbientLightColor");  
  shaderProgram.uniformDiffuseLightColorLoc = gl.getUniformLocation(shaderProgram, "uDiffuseLightColor");
  shaderProgram.uniformSpecularLightColorLoc = gl.getUniformLocation(shaderProgram, "uSpecularLightColor");
}


//-------------------------------------------------------------------------
function uploadLightsToShader(loc,a,d,s) {
  gl.uniform3fv(shaderProgram.uniformLightPositionLoc, loc);
  gl.uniform3fv(shaderProgram.uniformAmbientLightColorLoc, a);
  gl.uniform3fv(shaderProgram.uniformDiffuseLightColorLoc, d);
  gl.uniform3fv(shaderProgram.uniformSpecularLightColorLoc, s);
}

//----------------------------------------------------------------------------------
function setupBuffers() {
    setupTerrainBuffers();
    mat4.lookAt(mvMatrix,eyePt,viewPt,up);    
}

//----------------------------------------------------------------------------------
function draw() { 
  
    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // We'll use perspective 
    mat4.perspective(pMatrix,degToRad(45), gl.viewportWidth / gl.viewportHeight, 0.1, 200.0);

    setMatrixUniforms();
    
    uploadLightsToShader([0,1,1],[0.0,0.0,0.0],[1.0,0.5,0.0],[0.0,0.0,0.0]);
    drawTerrain();
   

}

//----------------------------------------------------------------------------------
function animate() {
    var timeNow = new Date().getTime();
	if(lastTime== -1)
		lastTime = timeNow;
	   var elapsed = (timeNow - lastTime) / 1000.0;
	lastTime = timeNow;

	var zRotation = 0;
	var yRotation = 0;
	var xRotation = 0;
	
	if(keysDown[83]) {
		// W, pitch up
		xRotation += elapsed * turnSpeed;
	}

	if(keysDown[87]) {
		// S, pitch down
		xRotation -= elapsed * turnSpeed;
	}

	if(keysDown[65]) {
		// A, turn left
		yRotation -= elapsed * turnSpeed;
	}

	if(keysDown[68]) {
		// D, turn right
		yRotation += elapsed * turnSpeed;
	}

	if(keysDown[81]) {
		// Q, tilt left
		zRotation -= elapsed * turnSpeed;
	}

	if(keysDown[69]) {
		// E, tilt right
		zRotation += elapsed * turnSpeed;
	}

	var a = mat4.create();
	var b = mat4.create();
	var c = mat4.create();
	mat4.copy(c, mvMatrix);

	mat4.rotateZ(a, mat4.create(), degToRad(zRotation));
	mat4.rotateY(b, a, degToRad(yRotation));
	mat4.rotateX(a, b, degToRad(xRotation));
	mat4.translate(b, a, [0, 0, elapsed * airSpeed]);
	mat4.multiply(mvMatrix, b, c);
   
}

//----------------------------------------------------------------------------------
function startup() {
  canvas = document.getElementById("myGLCanvas");
  gl = createGLContext(canvas);
  setupShaders();
  setupBuffers(); 
  gl.clearColor(0.484, 0.8906, 1.0, 1.0);
  gl.enable(gl.DEPTH_TEST);
  
    
  document.onkeydown = handleKeyDown;
  document.onkeyup = handleKeyUp;
  tick();
}

//----------------------------------------------------------------------------------
function tick() {
    requestAnimFrame(tick);
    draw();
    animate();
}
