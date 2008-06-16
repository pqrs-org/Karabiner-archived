void
%%LASTNAME%%(const RemapParams &params)
{
  if (! config.%%LASTNAME%%) return;

  RemapUtil::keyToKeyWithModifier(params, %%PARAMS%%);
}
