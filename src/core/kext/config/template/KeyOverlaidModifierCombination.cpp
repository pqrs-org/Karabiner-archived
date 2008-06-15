void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;

  static KeyOverlaidModifierCombination komc;
  komc.remap(params, %%PARAMS%%);
}
