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
	mat4    model_matrices[];
};

layout(binding = 1, std430) buffer CandidateDraws
{
    CandidateDraw candidateDraws[];
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
	const CandidateDraw draw = candidateDraws[gl_GlobalInvocationID.x];
    const mat4 model_matrix = model_matrices[gl_GlobalInvocationID.x];

	vec4 position = view_proj_matrix * model_matrix * vec4(draw.sphere.xyz, 1.0);

	if (all(lessThan(abs(position.xy) - draw.sphere.ww, position.ww * 1.0)))	//if ((abs(position.x) - thisDraw.sphere.w) < (position.w * 0.5) && (abs(position.y) - thisDraw.sphere.w) < (position.w * 0.5) &&

	{
        uint outDrawIndex = atomicCounterIncrement(commandCounter);

        command[outDrawIndex].indexCount = draw.indexCount;
        command[outDrawIndex].instanceCount = 1;
		command[outDrawIndex].firstIndex = 0;
		command[outDrawIndex].baseVertex = 0;
		command[outDrawIndex].baseInstance = uint(gl_GlobalInvocationID.x);
    }
}
