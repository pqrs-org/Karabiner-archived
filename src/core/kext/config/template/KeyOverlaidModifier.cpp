void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;
  static KeyOverlaidModifier kom;
  kom.remap(params, %%PARAMS%%);
}
