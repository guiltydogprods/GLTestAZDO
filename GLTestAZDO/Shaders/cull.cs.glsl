#version 440 core

layout (local_size_x = 16) in;

struct CandidateDraw
{
	vec3 aabbMin;
	uint firstIndex;
	vec3 aabbMax;
    uint indexCount;
	uint material_id;
};

struct DrawElementsIndirectCommand
{
    uint indexCount;
    uint instanceCount;
    uint firstIndex;
	uint baseVertex;
    uint baseInstance;
};

layout(binding = 0, std430) readonly buffer MODEL_MATRIX_BLOCK
{
	mat4    model_matrices[];
};

layout(binding = 1, std430) readonly buffer CandidateDraws
{
    CandidateDraw candidateDraws[];
};

layout (binding = 2, std430) writeonly buffer OutputDraws
{
    DrawElementsIndirectCommand command[];
};

layout (binding = 3, std430) writeonly buffer OutputMatrices
{
	mat4	visible_model_matrices[];
};

layout (binding = 0, std140) uniform TRANSFORM_BLOCK
{
    mat4    view_matrix;
    mat4    proj_matrix;
    mat4    view_proj_matrix;
};

layout (binding = 0, offset = 0) uniform atomic_uint commandCounter;

void main(void)
{
	const CandidateDraw draw = candidateDraws[gl_GlobalInvocationID.x];
    const mat4 model_matrix = model_matrices[gl_GlobalInvocationID.x];

	
	vec3 points[8];
	points[0] = vec3(draw.aabbMin.x, draw.aabbMin.y, draw.aabbMin.z);
	points[1] = vec3(draw.aabbMax.x, draw.aabbMin.y, draw.aabbMin.z);
	points[2] = vec3(draw.aabbMax.x, draw.aabbMax.y, draw.aabbMin.z);
	points[3] = vec3(draw.aabbMin.x, draw.aabbMax.y, draw.aabbMin.z);
	points[4] = vec3(draw.aabbMin.x, draw.aabbMin.y, draw.aabbMax.z);
	points[5] = vec3(draw.aabbMax.x, draw.aabbMin.y, draw.aabbMax.z);
	points[6] = vec3(draw.aabbMax.x, draw.aabbMax.y, draw.aabbMax.z);
	points[7] = vec3(draw.aabbMin.x, draw.aabbMax.y, draw.aabbMax.z);
	for (uint i = 0; i < 8; ++i)
	{
		vec4 position = view_proj_matrix * model_matrix * vec4(points[i], 1.0);
		if (all(lessThan(abs(position.xyz), position.www)))		//if ((abs(position.x) - thisDraw.sphere.w) < (position.w * 0.5) && (abs(position.y) - thisDraw.sphere.w) < (position.w * 0.5) && (abs(position.z) - thisDraw.sphere.w) < (position.w * 0.5)
		{		
			uint outDrawIndex = atomicCounterIncrement(commandCounter);

			// Write out Draw Command.
			command[outDrawIndex].indexCount = draw.indexCount;
			command[outDrawIndex].instanceCount = 1;
			command[outDrawIndex].firstIndex = 0;
			command[outDrawIndex].baseVertex = 0;
			command[outDrawIndex].baseInstance = draw.material_id;
			// Write out Model Matrix.
			visible_model_matrices[outDrawIndex] = model_matrix;
			break;
		}
	}
}
