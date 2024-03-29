g_drawOnce = true;
    g_debug = true;

    var aCoords;           // Location of the coords attribute variable in the shader program.
    var uProjection;       // Location of the projection uniform matrix in the shader program.
    var uModelview;

    var projection = mat4.create();   // projection matrix
    var modelview;    // modelview matrix

    var rotator;   // A SimpleRotator object to enable rotation by mouse dragging.

    var texID;
    var cube;
    var g_skyBoxUrls = [
        'images/pos-x.png',
        'images/neg-x.png',
        'images/pos-y.png',
        'images/neg-y.png',
        'images/pos-z.png',
        'images/neg-z.png'
        ];

    var lastTime = new Date().getTime();
    var gl;

    // extend vec3 for debugging
    vec3.toString = function (v) {
        return v[0] + ', ' + v[1] + ', ' + v[2];
    };

    vec3.divideByScalar = function(out, a, scalar) {
        out[0] = a[0] / scalar;
        out[1] = a[1] / scalar;
        out[2] = a[2] / scalar;
        return out;
    };

    function degToRad(degrees) {
        return degrees * Math.PI / 180;
    }

    function randPos(scale) {
        scale = scale || 1;
        return vec3.random(vec3.create(), scale);
    }

    function initGL(canvas) {
        try {
            gl = canvas.getContext("experimental-webgl");
            gl.viewportWidth = canvas.width;
            gl.viewportHeight = canvas.height;
        } catch (e) {
        }
        if (!gl) {
            alert("Could not initialise WebGL, sorry :-(");
        }
    }


    function getShader(gl, id) {
        var shaderScript = document.getElementById(id);
        if (!shaderScript) {
            return null;
        }

        var str = "";
        var k = shaderScript.firstChild;
        while (k) {
            if (k.nodeType == 3) {
                str += k.textContent;
            }
            k = k.nextSibling;
        }

        var shader;
        if (shaderScript.type == "x-shader/x-fragment") {
            shader = gl.createShader(gl.FRAGMENT_SHADER);
        } else if (shaderScript.type == "x-shader/x-vertex") {
            shader = gl.createShader(gl.VERTEX_SHADER);
        } else {
            return null;
        }

        gl.shaderSource(shader, str);
        gl.compileShader(shader);

        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            alert(gl.getShaderInfoLog(shader));
            return null;
        }

        return shader;
    }


    var shaderProgram;

    function initShaders() {
        var fragmentShader = getShader(gl, "skyboxFragmentShader");
        var vertexShader = getShader(gl, "skyboxVertexShader");

        shaderProgram = gl.createProgram();
        gl.attachShader(shaderProgram, vertexShader);
        gl.attachShader(shaderProgram, fragmentShader);
        gl.linkProgram(shaderProgram);

        if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
            alert("Could not initialise shaders");
        }

        gl.useProgram(shaderProgram);
    }


    function initBuffers(canvas) {
        aCoords =  gl.getAttribLocation(shaderProgram, "coords");
        uModelview = gl.getUniformLocation(shaderProgram, "modelview");
        uProjection = gl.getUniformLocation(shaderProgram, "projection");

        gl.enableVertexAttribArray(aCoords);
        gl.enable(gl.DEPTH_TEST);

        rotator = new SimpleRotator(canvas, drawScene);
        rotator.setView( [0,0,1], [0,1,0], 5 );
        cube = createModel(cube(200));
    }

    function loadTextureCube(urls) {
        var ct = 0;
        var img = new Array(6);

        for (var i = 0; i < 6; i++) {
            img[i] = new Image();
            img[i].onload = function() {
                ct++;
                if (ct == 6) {
                    texID = gl.createTexture();
                    gl.bindTexture(gl.TEXTURE_CUBE_MAP, texID);

                    var targets = [
                        gl.TEXTURE_CUBE_MAP_POSITIVE_X, gl.TEXTURE_CUBE_MAP_NEGATIVE_X, 
                        gl.TEXTURE_CUBE_MAP_POSITIVE_Y, gl.TEXTURE_CUBE_MAP_NEGATIVE_Y, 
                        gl.TEXTURE_CUBE_MAP_POSITIVE_Z, gl.TEXTURE_CUBE_MAP_NEGATIVE_Z 
                            ];

                    for (var j = 0; j < 6; j++) {
                        gl.texImage2D(targets[j], 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img[j]);
                        gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
                        gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
                    }
                    gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
                    drawScene();
                }
            }
            img[i].src = urls[i];
        }
    }

    function createModel(modelData) {
        var model = {};

        model.coordsBuffer = gl.createBuffer();
        model.indexBuffer = gl.createBuffer();
        model.count = modelData.indices.length;

        gl.bindBuffer(gl.ARRAY_BUFFER, model.coordsBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, modelData.vertexPositions, gl.STATIC_DRAW);

        console.log(modelData.vertexPositions.length);
        console.log(modelData.indices.length);

        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, model.indexBuffer);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, modelData.indices, gl.STATIC_DRAW);

        model.render = function() { 
            gl.bindBuffer(gl.ARRAY_BUFFER, this.coordsBuffer);
            gl.vertexAttribPointer(aCoords, 3, gl.FLOAT, false, 0, 0);
            gl.uniformMatrix4fv(uModelview, false, modelview );
            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
            gl.drawElements(gl.TRIANGLES, this.count, gl.UNSIGNED_SHORT, 0);
            console.log(this.count);
        }
        return model;
    }

    /**
     * Sets up the Skybox
     */
    function setupSkybox() {
        loadTextureCube(g_skyBoxUrls);
    }

    function drawScene() {
        gl.clearColor(0,0,0,1);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        //mat4.perspective(pMatrix, 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0);

        mat4.perspective(projection, Math.PI/3, 1, 50, 200);
        gl.uniformMatrix4fv(uProjection, false, projection );

        modelview = rotator.getViewMatrix();

        if (texID)
            cube.render();  
    }


    function animate() {
        var timeNow = new Date().getTime();

        if (lastTime != 0) {
            var elapsed = timeNow - lastTime;
            var dt = elapsed / 1000;
        }
        lastTime = timeNow;
    }

    function tick() {
        if (!g_drawOnce) {
            requestAnimFrame(tick);
        }
        drawScene();
        animate();
    }


    function webGLStart() {
        canvas = document.querySelector('#experimental-webgl');
        initGL(canvas);
        initShaders()
        initBuffers(canvas);
        setupSkybox();

        tick();
    }