inline void
%%LASTNAME%%(const RemapConsumerParams &params)
{
  if (! config.%%LASTNAME%%) return;

  RemapUtil::consumerToKey(params, %%PARAMS%%);
}
