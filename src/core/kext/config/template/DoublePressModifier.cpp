inline void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;

  static DoublePressModifier dpm;
  dpm.remap(params, %%PARAMS%%);
}
