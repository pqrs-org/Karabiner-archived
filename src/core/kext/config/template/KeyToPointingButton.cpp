inline void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;

  RemapUtil::keyToPointingButton(params, %%PARAMS%%);
}
