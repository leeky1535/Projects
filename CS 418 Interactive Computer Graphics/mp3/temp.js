function computeNormsTexCoords(data) {
	// the plan is to find the normal of each triangle, then go through all the vertices, find all the triangles they're attached to, and compute the per-vertex normal from there
	var numVertices = data.vertexPositions.length / 3;
	var numTris = data.indices.length / 3;

	data.vertexNormals = new Array();

	// will contain the xyz components of the normal
	var triangles = new Array(numTris);
	// vertexIndices[n] will contain the indices of the triangles that vertex[n] is part of
	var vertexIndices = new Array(numVertices);
	for(var i = 0; i < vertexIndices.length; i++)
		vertexIndices[i] = new Array();

    var u = vec3.create();
	var v = vec3.create();

	for(var i = 0; i < numTris; i++) {
		// indices of the indices of the vertices
		var vii1 = 3 * i;
		var vii2 = 3 * i + 1;
		var vii3 = 3 * i + 2;
		// indices of the vertices
		var vi1 = data.indices[vii1] * 3;
		var vi2 = data.indices[vii2] * 3;
		var vi3 = data.indices[vii3] * 3;
		// vertices
		var v1 = [data.vertexPositions[vi1], data.vertexPositions[vi1 + 1], data.vertexPositions[vi1 + 2]];
		var v2 = [data.vertexPositions[vi2], data.vertexPositions[vi2 + 1], data.vertexPositions[vi2 + 2]];
		var v3 = [data.vertexPositions[vi3], data.vertexPositions[vi3 + 1], data.vertexPositions[vi3 + 2]];

		
		var normal = vec3.create();
		var normalized = vec3.create();
		vec3.subtract(u, v2, v1);
		vec3.subtract(v, v3, v1);
		vec3.cross(normal, u, v);
		vec3.normalize(normalized, normal);

		// save this vector
		triangles[i] = normalized;
		// save the vertices it's part of
		vertexIndices[vi1 / 3].push(i);
		vertexIndices[vi2 / 3].push(i);
		vertexIndices[vi3 / 3].push(i);
	}

	for(var i = 0; i < numVertices; i++) {
		var totalNormal = vec3.create();
		var temp = vec3.create();
		while(vertexIndices[i].length !== 0) {
			var currentTriangle = vertexIndices[i].pop();
			vec3.add(temp, totalNormal, triangles[currentTriangle]);
			vec3.copy(totalNormal, temp);
		}
		var normalized = vec3.create();
		vec3.normalize(normalized, totalNormal);
		data.vertexNormals[i * 3] = normalized[0];
		data.vertexNormals[i * 3 + 1] = normalized[1];
		data.vertexNormals[i * 3 + 2] = normalized[2];
	}

	for(var i = 0; i < numVertices; i++) {
		// angle should be atan(x/z)
		var angle = Math.atan(data.vertexPositions[3 * i] / data.vertexPositions[3 * i + 2]);
		data.vertexTextureCoords[2 * i] = Math.sin((angle + Math.PI / 4) / 2);
		data.vertexTextureCoords[2 * i + 1] = data.vertexPositions[3 * i + 1] * 7;
	}

	return data;
}

function handleLoadedModel(data, model) {
	model.normalBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, model.normalBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data.vertexNormals), gl.STATIC_DRAW);
	model.normalBuffer.itemSize = 3;
	model.normalBuffer.numItems = data.vertexNormals.length / 3;
	console.log("found " + model.normalBuffer.numItems + " normals");

    model.coordsBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, model.coordsBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data.vertexPositions), gl.STATIC_DRAW);
	model.coordsBuffer.itemSize = 3;
	model.coordsBuffer.numItems = data.vertexPositions.length / 3;
	console.log("found " + model.coordsBuffer.numItems + " vertices");

	model.indexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, model.indexBuffer);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(data.indices), gl.STATIC_DRAW);
	model.indexBuffer.itemSize = 1;
	model.indexBuffer.numItems = data.indices.length;
	console.log("found " + model.indexBuffer.numItems / 1 + " indices");
}

function loadTeapotModel() {
	$.get('teapot_0.obj', function(data) {
		handleLoadedModel(computeNormsTexCoords(obj2json(data)));
		ready = true;
	});
}