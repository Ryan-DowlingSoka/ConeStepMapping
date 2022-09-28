// Copyright (c) 2022 Ryan DowlingSoka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// THIS SOFTWARE WILL NOT BE SOLD FOR COMMERCIAL GAIN ON THE EPIC
// GAMES MARKETPLACE WITHOUT EXPRESS PERMISSION FROM THE COPYRIGHT HOLDER.
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Update given number of cells in a single iteration.
int num_to_visit = NumToVisit;
int start_num = TickCount * num_to_visit;
int end_num = min(start_num + num_to_visit, NumCellsX * NumCellsY);
for(int i = start_num; i < end_num; i++)
{
    int x = i % NumCellsX;
    int y = i / NumCellsX;
    
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
float3 offset;
offset.x = (TickCount % (NumCellsX*2)) / float(NumCellsX) / 2;
offset.y = (TickCount / (NumCellsX*2)) / float(NumCellsY) / 2;
offset.xy -= (0.5,0.5);
offset.zy *= (2,2);

//////////////////////////////////////////////////////////////////////////////////////////////
int search_steps = 256;
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

RelaxedCone = min(cone_ratio, best_ratio);
/////////////////////////////////////////////////////////////////////////////////////////////////////