#version 440 core

layout (local_size_x = 16) in;

struct CandidateDraw
{
	vec4 sphere;
    uint firstIndex;
    uint indexCount;
};

struct DrawElementsIndirectCommand
{
    uint indexCount;
    uint instanceCount;
    uint firstIndex;
	uint baseVertex;
    uint baseInstance;
};

layout(binding = 0, std140) readonly buffer MODEL_MATRIX_BLOCK
{
	mat4    model_matrix[];
};

layout(binding = 1, std430) buffer CandidateDraws
{
    CandidateDraw draw[];
};

layout (binding = 2, std430) writeonly buffer OutputDraws
{
    DrawElementsIndirectCommand command[];
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
    const CandidateDraw thisDraw = draw[gl_GlobalInvocationID.x];
    const mat4 thisModelMatrix = model_matrix[gl_GlobalInvocationID.x];

    vec4 position = view_proj_matrix * thisModelMatrix * vec4(thisDraw.sphere.xyz, 1.0);

	if ((abs(position.x) - thisDraw.sphere.w) < (position.w * 1.0) &&
		(abs(position.y) - thisDraw.sphere.w) < (position.w * 1.0))
	{
        uint outDrawIndex = atomicCounterIncrement(commandCounter);

        command[outDrawIndex].indexCount = thisDraw.indexCount;
        command[outDrawIndex].instanceCount = 1;
		command[outDrawIndex].firstIndex = 0;
		command[outDrawIndex].baseVertex = 0;
		command[outDrawIndex].baseInstance = uint(gl_GlobalInvocationID.x);
    }
}
