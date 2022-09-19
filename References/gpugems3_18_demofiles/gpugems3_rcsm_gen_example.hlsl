 float4 depth2relaxedcone(
    in float2 TexCoord : TEXCOORD0,
    in Sampler2D ReliefSampler,
    in float3 Offset ) : COLOR 
{
    const int search_steps = 128;
    float3 src = float3(TexCoord,0);  // Source texel
    float3 dst = src + Offset;  // Destination texel
    dst.z = tex2D(ReliefSampler,dst.xy).w;  // Set dest. depth
    float3 vec = dst - src; // Ray direction
    vec /= vec.z;  // Scale ray direction so that vec.z = 1.0
    vec *= 1.0 - dst.z;  // Scale again
    float3 step_fwd = vec/search_steps;  // Length of a forward step
    // Search until a new point outside the surface
    float3 ray_pos = dst + step_fwd;
    for( int i=1; i<search_steps; i++ )
    {
        float current_depth = tex2D(ReliefSampler, ray_pos.xy).w;
        if ( current_depth <= ray_pos.z )
            ray_pos += step_fwd;
    }
    // Original texel depth
    float src_texel_depth = tex2D(ReliefSampler,TexCoord).w;
    // Compute the cone ratio
    float cone_ratio = (ray_pos.z >= src_texel_depth) ? 1.0 : length(ray_pos.xy - TexCoord) / (src_texel_depth - ray_pos.z);
    // Check for minimum value with previous pass result
    float best_ratio = tex2D(ResultSampler, TexCoord).x;
    if ( cone_ratio > best_ratio )
        cone_ratio = best_ratio;
    return float4(cone_ratio, cone_ratio, cone_ratio, cone_ratio); 
} 
