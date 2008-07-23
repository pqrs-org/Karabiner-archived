inline void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;

  RemapUtil::keyToKey(params, %%PARAMS%%);
}
