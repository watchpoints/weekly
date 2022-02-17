
// Execute the physical plan.
logStmt(stmt, s.sessionVars)
recordSet, err := runStmt(ctx, s, stmt)
if err != nil {
	if !kv.ErrKeyExists.Equal(err) {
		logutil.Logger(ctx).Warn("run statement failed",
			zap.Int64("schemaVersion", s.sessionVars.TxnCtx.SchemaVersion),
			zap.Error(err),
			zap.String("session", s.String()))
	}
	return nil, err
}
	
	
if execStmt, ok := s.(*ast.ExecuteStmt); ok {
	s, err = planner.GetPreparedStmt(execStmt, vars)
	if err != nil {
		return
	}
}