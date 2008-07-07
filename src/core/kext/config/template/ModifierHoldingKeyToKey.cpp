void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;

  static ModifierHoldingKeyToKey mhkk;
  mhkk.remap(params, %%PARAMS%%);
}
