
/////////////////////////////////////////////////////////////////////////////////////////////////////
//Update in one iteration.
for(int x = 0; x < NumCellsX; x++)
{
    for(int y = 0; y < NumCellsY; y++)
    {
        float3 offset;
        offset.x = x / float(NumCellsX);
        offset.y = y / float(NumCellsY);
        offset.xy -= (0.5,0.5);
        offset.xy *= (1,1);

        //////////////////////////////////////////////////////////////////////////////////////////////
        int search_steps = 128;
        float3 src = float3(UV.x, UV.y, 0.0f);
        float3 dst = src+offset;
        float out_height;
        SamplePreviousGridFloatValue_Emitter_SourceGrid_AttributeHeight(frac(dst.xy), out_height);
        dst.z = out_height;
        float3 vec = dst - src;
        vec = vec / max(vec.z,0.0001f);
        vec = vec * (1.0 - dst.z);

        vec = vec / search_steps;
        float3 step_fwd = vec;
        float3 ray_pos = dst + step_fwd;

        for( int i =1; i<search_steps; i++ )
        {
            float current_depth = 0.0f;
            SamplePreviousGridFloatValue_Emitter_SourceGrid_AttributeHeight(frac(ray_pos.xy), current_depth);
            if( current_depth <= ray_pos.z )
                ray_pos += step_fwd;
        }


        float cone_ratio = (ray_pos.z >= src_texel_depth) ? 1.0 : length(ray_pos.xy - UV) / (src_texel_depth - ray_pos.z);

        best_ratio = min(cone_ratio, best_ratio);
    }
}

RelaxedCone = best_ratio;
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
//Update target line once per iteration.

for(int x = 0; x < (NumCellsX); x++)
{
    float3 offset;
    offset.x = x / float(NumCellsX);
    offset.y = TickCount / float(NumCellsY);
    offset.xy -= (0.5,0.5);
    offset.xy *= (1,1);

    //////////////////////////////////////////////////////////////////////////////////////////////
    int search_steps = 128;
    float3 src = float3(UV.x, UV.y, 0.0f);
    float3 dst = src+offset;
    float out_height;
    SamplePreviousGridFloatValue_Emitter_SourceGrid_AttributeHeight(frac(dst.xy), out_height);
    dst.z = out_height;
    float3 vec = dst - src;
    vec = vec / max(vec.z,0.0001f);
    vec = vec * (1.0 - dst.z);

    vec = vec / search_steps;
    float3 step_fwd = vec;
    float3 ray_pos = dst + step_fwd;

    for( int i =1; i<search_steps; i++ )
    {
        float current_depth = 0.0f;
        SamplePreviousGridFloatValue_Emitter_SourceGrid_AttributeHeight(frac(ray_pos.xy), current_depth);
        if( current_depth <= ray_pos.z )
            ray_pos += step_fwd;
    }


    float cone_ratio = (ray_pos.z >= src_texel_depth) ? 1.0 : length(ray_pos.xy - UV) / (src_texel_depth - ray_pos.z);

    best_ratio = min(cone_ratio, best_ratio);
}

RelaxedCone = best_ratio;
/////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////
//Update one target per iteration.
// TODO: restore this properly.
float3 offset;
offset.x = TickCount % (NumCellsX*2) / float(NumCellsX) / 2;
offset.y = TickCount / float(NumCellsY) / 2;
offset.xy -= (0.5,0.5);
offset.xy *= (1,1);

//////////////////////////////////////////////////////////////////////////////////////////////
int search_steps = 128;
float3 src = float3(UV.x, UV.y, 0.0f);
float3 dst = src+offset;
float out_height;
SamplePreviousGridFloatValue_Emitter_SourceGrid_AttributeHeight(frac(dst.xy), out_height);
dst.z = out_height;
float3 vec = dst - src;
vec = vec / max(vec.z,0.0001f);
vec = vec * (1.0 - dst.z);

vec = vec / search_steps;
float3 step_fwd = vec;
float3 ray_pos = dst + step_fwd;

for( int i =1; i<search_steps; i++ )
{
    float current_depth = 0.0f;
    SamplePreviousGridFloatValue_Emitter_SourceGrid_AttributeHeight(frac(ray_pos.xy), current_depth);
    if( current_depth <= ray_pos.z )
        ray_pos += step_fwd;
}


float cone_ratio = (ray_pos.z >= src_texel_depth) ? 1.0 : length(ray_pos.xy - UV) / (src_texel_depth - ray_pos.z);

best_ratio = min(cone_ratio, best_ratio);

RelaxedCone = best_ratio;
/////////////////////////////////////////////////////////////////////////////////////////////////////